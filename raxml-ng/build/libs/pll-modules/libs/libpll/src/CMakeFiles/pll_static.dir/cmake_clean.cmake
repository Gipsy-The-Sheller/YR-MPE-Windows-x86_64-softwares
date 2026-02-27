file(REMOVE_RECURSE
  "libpll.a"
  "libpll.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/pll_static.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
