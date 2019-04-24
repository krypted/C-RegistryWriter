#include <iostream>
#include <string>
#include "RegistryKeyManager.h"
using namespace std;

#define DEFAULT_PATH  "hkey_current_user/Software/krypted/token"
#define DEFAULT_KEYVAL "test"

/// <summary> The main program demonstrates using the RegistryKeyManager class with command line arguments.</summary>
int main( int argc, char* argv[] )
{
	RegistryKeyManager rkm;

	// No arguments - read information directly from user input (or use defaults).
	if ( argc < 2 )
	{
		/*
			Read key location/name
		*/
		string path;
		cout << "Enter key path (default: \"" << DEFAULT_PATH << "\"): ";
		getline( std::cin, path );

		if ( !path.empty() )
			cout << " You entered: `" << path << "' " << endl;
		else
		{
			path = DEFAULT_PATH;
			cout << "Using default key path value" << endl;
		}

		/*
			Read key value
		*/
		string value; // will be converted to DWORD inside the RegistryKeyManager class.
		cout << "Enter key value (default: \"" << DEFAULT_KEYVAL << "\"): ";
		getline( std::cin, value );

		if( !value.empty() )
			cout << " You entered: `" << value << "' " << endl;
		else
		{
			value = DEFAULT_KEYVAL;
			cout << "Using default DWORD value." << endl;
		}
		/*
			Reonstruct RegistryKeyManager class with read data.
		*/
		rkm = RegistryKeyManager( path, RegistryKeyManager::ToDword( value.c_str() ) );

		/*
			Test reading/writing registry key.
		*/

		cout << endl << endl;
		cout << "**** Testing registry key `" << path << "' **** " << endl;
		cout << "Reading current value: `" << RegistryKeyManager::ToString( rkm.ReadDwordValue() ) << "' " << endl;

		cout << endl;
		cout << "Writing new value: `AaBbCcDd' " << endl;
		rkm.WriteValue( RegistryKeyManager::ToDword( "AaBb" ) );
		cout << "Reading current value: `" << RegistryKeyManager::ToString( rkm.ReadDwordValue() ) << "' " << endl;
		cout << endl;

		system( "pause" );
	}

	// Create a new instance of the RegistryKeyManager class with the supplied argument.
	else
	{
		std::string path( DEFAULT_PATH );
		DWORD value = RegistryKeyManager::ToDword( DEFAULT_KEYVAL );

		// Usage example: 
		// -k hkey_current_user/software/krypted/token -v test1234 

		for ( int i = 0; i < argc - 1; i++ )
			if ( argv[i] == CLARG_PATH )
				path = argv[++i];
			else if ( argv[i] == CLARG_KEYVAL )
				value = RegistryKeyManager::ToDword( argv[++i] );

		rkm = RegistryKeyManager( path, value);
	}
	return 0;
}