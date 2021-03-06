#include <crails/front/element.hpp>

using namespace std;
using namespace Crails::Front;

Element::Element()
{
  el = client::document.createElement("div");
}

Element::Element(const client::String& type, const map<string, string>& children)
{
  el = client::document.createElement(type);
  attr(children);
}

Element::Element(client::HTMLElement* el)
{
  this->el = el;
}

void Element::destroy()
{
  if (has_parent())
    el->get_parentElement()->removeChild(el);
}

bool Element::is_visible() const
{
  auto* style       = el->get_style();
  auto* css_display = style->get_display();
  auto* css_visible = style->get_visibility();

  return (!css_display || (string)(*css_display) != "none") &&
         (!css_visible || (string)(*css_visible) != "hidden");
}

bool Element::has_parent() const
{
  return el->get_parentElement() != 0;
}

Element Element::get_parent()
{
  return Element(el->get_parentElement());
}

Element Element::get_next()
{
  return Element((client::HTMLElement*)el->get_nextElementSibling());
}

Element& Element::inner(const std::vector<Element>& children)
{
  for (Element element : children)
    el->appendChild(element.el);
  return *this;
}

Element& Element::operator>(const std::vector<Element>& children)
{
  return inner(children);
}

Element& Element::operator>(Element child)
{
  return inner({ child });
}

Element& Element::attr(const std::map<std::string, std::string>& attrs)
{
  for (auto it = attrs.begin() ; it != attrs.end() ; ++it)
   el->setAttribute(it->first.c_str(), it->second.c_str());
  return *this;
}

Element& Element::attr(const std::map<std::string, std::wstring>& attrs)
{
  for (auto it = attrs.begin() ; it != attrs.end() ; ++it)
   el->setAttribute(it->first.c_str(), it->second.c_str());
  return *this;
}

Element& Element::attr(const std::map<std::string, client::String*>& attrs)
{
  for (auto it = attrs.begin() ; it != attrs.end() ; ++it)
   el->setAttribute(it->first.c_str(), it->second);
  return *this;
}

void Element::append_to(client::HTMLElement* el)
{
  el->appendChild(this->el);
}

bool Element::contains(const client::HTMLElement* source)
{
  bool result = false;

  if (el == source)
    return true;
  each([&result, source](Element& el) -> bool
  {
    result = el.contains(source);
    return !result;
  });
  return result;
}

std::vector<Element> Element::find(const std::string& selector)
{
  auto* node_list = el->querySelectorAll(selector.c_str());
  std::vector<Element> results;

  results.resize(node_list->get_length());
  for (double i = 0 ; i < node_list->get_length() ; ++i)
    results[i] = Element(static_cast<client::HTMLElement*>(node_list->item(i)));
  return results;
}

void Element::each(std::function<bool (Element&)> func)
{
  auto* node_list = el->get_childNodes();

  for (double i = 0 ; i < node_list->get_length() ; ++i)
  {
    Element child(static_cast<client::HTMLElement*>(node_list->item(i)));

    if (func(child) == false)
      break ;
  }
}

Element& Element::empty()
{
  auto* node_list = el->get_childNodes();

  while (node_list->get_length())
  {
    el->removeChild(node_list->item(0));
    node_list = el->get_childNodes();
  }
  return *this;
}

void Element::toggle_class(const std::string& str, bool set)
{
  if (has_class(str))
  {
    if (!set)
      remove_class(str);
  }
  else if (set)
    add_class(str);
}

void Element::add_class(const std::string& str)
{
  client::String* original = el->getAttribute("class");
  std::string result;

  if (original)
    result = (std::string)*original;
  if (result.size() > 0)
    result += ' ';
  result += str;
  el->setAttribute("class", result.c_str());
}

void Element::remove_class(const std::string& str)
{
  client::String* original = el->getAttribute("class");

  if (original)
  {
    std::string result = (std::string)*original;
    auto it = result.find(str);
    if (it > 0)
      result.erase(it - 1, it + str.size() + 1);
    else
      result.erase(it, it + str.size() + 1);
    el->setAttribute("class", result.c_str());
  }
}

bool Element::has_class(const std::string& str) const
{
  client::String* class_ptr = el->getAttribute("class");

  if (class_ptr)
  {
    std::string classes = (std::string)*class_ptr;

    return classes.find(str) != std::string::npos;
  }
  return false;
}

bool Element::has_attribute(const std::string& key) const
{
  return el->hasAttribute(key.c_str());
}

std::string Element::get_attribute(const std::string& key) const
{
  client::String* attr_ptr = el->getAttribute(key.c_str());

  return (std::string)(*attr_ptr);
}

void Element::remove_attribute(const std::string& key)
{
  el->removeAttribute(key.c_str());
}

std::string Element::get_value() const
{
  auto* input_el      = static_cast<client::HTMLInputElement*>(el);
  auto* client_string = input_el->get_value();

  return (std::string)(*client_string);
}
