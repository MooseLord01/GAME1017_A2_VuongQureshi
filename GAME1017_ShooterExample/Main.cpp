/*******************************************************************************
* File Name   : Main.cpp 
* Description : Assignment 1 for GAME 1017
* Author      :
* Created     :
* Modified    :
*******************************************************************************/

// SDL includes pasted for convenience. Move/copy to relevant files.
#include "SDL.h"
#include "Engine.h"

int main(int argc, char* argv[])
{
	return Engine::Instance().Run();
}