#pragma once
#include "globals.h"
#include "Utility.h"
#include "ClientUtils.h"

int main(int argc, char** argv) {
	char* runType = argv[1];
	if (!strcmp(CLIENT_MODE, runType)) {
		runClientMode(argc, argv);
	}
}