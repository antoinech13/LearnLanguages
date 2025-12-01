# CMakeList.txt : projet CMake pour learn_languages, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (learn_languages "learn_languages.cpp" "learn_languages.h" "utils.cpp" "utils.h" "fileParser2.cpp" "fileParser2.h" "manager.cpp" "manager.h" "dataStruct.h")
#"fileParser.cpp" "fileParser.h" "manager.cpp" "manager.h"
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET learn_languages PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
