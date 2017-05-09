#include "../generic_list.h"

// Creates itself

// Thread 1: Resolves client requests in client socket
	// Sends an address back according to the roud-robin approach

// (???) Not sure if 1 or 2 threads, 1 or 2 sockets
// Thread 2: 
	// Manages peer pings in peer socket
		// If one is dead, remove from the list
	// Manages peer info in peer socket
		// Adds it to the peer list

// Must have syncronization when reading/writing the list of peers
