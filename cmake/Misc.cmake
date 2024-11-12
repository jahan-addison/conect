
configure_file (
  "${PROJECT_SOURCE_DIR}/orianna/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
