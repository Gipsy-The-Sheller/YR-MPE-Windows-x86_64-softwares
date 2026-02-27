file(REMOVE_RECURSE
  "libpllmodtree.a"
  "libpllmodtree.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/pllmodtree_static.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
