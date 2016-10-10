# NOTES

## Dependencies when using as source library:

* GSL
  * Copy from [build-dir]/external/gsl-install/include and add it as include path
  * Be sure to configure `GSL_TERMINATE_ON_CONTRACT_VIOLATION`, `GSL_THROW_ON_CONTRACT_VIOLATION` or `GSL_UNENFORCED_ON_CONTRACT_VIOLATION`

Other dependencies are either used for the execuable (tclap, json) or for tests (catch)