file(REMOVE_RECURSE
  "libpllmodoptimize.a"
  "libpllmodoptimize.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/pllmodoptimize_static.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
