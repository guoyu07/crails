cmake_minimum_required(VERSION 3.0)

project(crails-json)

include_directories(.)

file(GLOB crails_json             src/json_renderer.cpp src/json_template.cpp)
file(GLOB guard_json_plugins      scripts/guard/*.rb)
file(GLOB scripts_json_templates  scripts/guard/templates/*.erb)
file(GLOB scripts_json_module     scripts/modules/json/*.rb)

add_library(crails-json${crails-suffix} SHARED ${crails_json})

install(DIRECTORY crails                 DESTINATION include)
install(FILES ${scripts_json_templates}  DESTINATION share/crails/guard/templates)
install(FILES ${scripts_json_module}     DESTINATION share/crails/scripts/modules/json)
install(FILES ${guard_json_plugins}      DESTINATION share/crails/guard)
install(TARGETS crails-json${crails-suffix} LIBRARY DESTINATION lib)
