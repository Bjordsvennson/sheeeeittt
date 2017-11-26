#pragma once

#include <iostream>
#include <vector>

class BaseNode
{
public:
	BaseNode();

private:
	int x, y, w, h;
	BaseNode* child;

};