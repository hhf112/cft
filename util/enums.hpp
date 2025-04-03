#pragma once

enum query {
  JUDGE,
  INIT,
  CLEAN,
  COUNT,

};

enum class status {
  AC,
  RUNTIME_ERR,
  WRONG_OUTPUT,
  WA,
  NILIO,
  UNKNOWN,
  PROCESSING_ERR,
};

enum class warning {
  TLE,
  GOOD,
};

enum class buildErr {
  PROCESSING_ERR,
  NULL_BS,
  BUILD_FAIL,
};
