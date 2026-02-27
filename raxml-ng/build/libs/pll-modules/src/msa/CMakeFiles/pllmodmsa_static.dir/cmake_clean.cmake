file(REMOVE_RECURSE
  "libpllmodmsa.a"
  "libpllmodmsa.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/pllmodmsa_static.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
