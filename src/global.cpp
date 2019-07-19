#include "../include/global.h"

global::global()  {}
global::~global() {}
global * global::global_ptr (nullptr);
global * global::get_ptr() {return global_ptr;}