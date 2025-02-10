#pragma warning (disable: 4005)
#pragma warning (disable: 26812)
#pragma warning (disable: 4244)

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <deque>
#include <array>
#include <vector>
#include <time.h>

#pragma comment (lib, "minhook/minhook.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11")

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include "math/martix4x4/matrix4x4.h"

#include "console.h"

