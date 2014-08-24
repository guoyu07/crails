var gulp   = require('gulp');
var coffee = require('gulp-coffee');
var concat = require('gulp-concat');
var uglify = require('gulp-uglify');
var sass   = require('gulp-sass');
var gulpif = require('gulp-if');
var exec   = require('gulp-exec');

var debug  = true;

var paths = {
  scripts: [ 'app/assets/javascript/**/*.js', 'app/assets/javascript/**/*.coffee' ],
  css:     [ 'app/assets/stylesheets/**/*.scss' ],
  ecpp:    [ 'app/views/**/*.ecpp', 'lib/*.ecpp' ]
}

gulp.task('scripts', ['clean'], function() {
  var scripts = gulp.src(paths.scripts).pipe(gulpif(/[.]coffee$/, coffee()));
  if (debug != true)
    scripts = scripts.pipe(uglify());
  return (scripts.pipe(concat('application.js')).pipe(gulp.dest('./public/assets')));
});

gulp.task('css', ['clean'], function() {
  return (gulp.src(paths.css).pipe(sass({ includePaths: paths.css })).pipe(concat('application.css')).pipe(gulp.dest('./public/assets')));
});

gulp.task('ecpp', ['clean'], function() {
  return (gulp.src(paths.ecpp).pipe(exec('crails compile_view <%= file.path %>'), { continueOnError: false, pipeStdout: false }).pipe(exec.reporter({ err: true, stderr: true, stdout: true })));
});

gulp.task('clean', function() {
});

gulp.task('watch', function() {
  gulp.watch(paths.scripts, ['scripts']);
  gulp.watch(paths.css,     ['css']);
  gulp.watch(paths.ecpp,    ['ecpp']);
});

gulp.task('default', ['watch', 'scripts', 'css', 'ecpp']);