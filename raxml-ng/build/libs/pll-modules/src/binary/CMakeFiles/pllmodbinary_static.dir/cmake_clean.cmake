file(REMOVE_RECURSE
  "libpllmodbinary.a"
  "libpllmodbinary.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/pllmodbinary_static.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
