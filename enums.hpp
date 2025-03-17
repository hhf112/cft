#pragma once

enum query {
  JUDGE,
  INIT,
  CLEAN,

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
