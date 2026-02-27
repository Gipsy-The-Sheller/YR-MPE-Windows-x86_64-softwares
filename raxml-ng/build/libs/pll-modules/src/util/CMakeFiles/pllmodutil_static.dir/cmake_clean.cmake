file(REMOVE_RECURSE
  "libpllmodutil.a"
  "libpllmodutil.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/pllmodutil_static.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
