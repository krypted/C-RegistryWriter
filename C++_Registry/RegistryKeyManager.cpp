#include <algorithm>
#include <map>
#include <iostream>
#include <assert.h>
#include "RegistryKeyManager.h"
using namespace std;	

#pragma region Opening and closing keys

LSTATUS RegistryKeyManager::Open()
{
	// We first attempt to open the key.
	LSTATUS keyStatus = RegOpenKeyEx( m_RootKey, m_Name.c_str(), NULL, KEY_ALL_ACCESS, &m_Key );

	// Registry key was not found; create it.
	if ( keyStatus == ERROR_FILE_NOT_FOUND )
		keyStatus = RegCreateKeyEx( m_RootKey, m_Name.c_str(), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_Key, NULL );

	// Make sure that keyStatus is 
	if ( keyStatus != ERROR_SUCCESS )
		cerr << "Unable to open or create key." << endl;

	return keyStatus;
}

LSTATUS RegistryKeyManager::Close()
{
	return RegCloseKey( m_Key );
}

#pragma endregion

#pragma region Reading and writing the value.

DWORD RegistryKeyManager::ReadDwordValue()
{
	DWORD data;

	if ( Open() == ERROR_SUCCESS )
	{
		LPCTSTR value = NULL;
		DWORD size = sizeof( data );
		DWORD type = REG_DWORD;

		LONG error = RegQueryValueEx( m_Key, "", NULL, &type, (LPBYTE)&data, &size );

		if ( error == ERROR_FILE_NOT_FOUND )
		{
			data = 0;
			cerr << "Key not found." << endl;
		}

		Close();

		return data;

	} // Key opened successfully.

	return 0;
}

BOOL RegistryKeyManager::WriteValue( const DWORD data )
{
	LPCTSTR value = NULL; 
	if ( Open() == ERROR_SUCCESS )
	{
		BOOL ret = RegSetValueEx( m_Key, value, NULL, REG_DWORD, (LPBYTE)&data, sizeof( DWORD ) );
		Close();
		return ret;
	}
	return false;
}

#pragma endregion

#pragma region Conversions

DWORD RegistryKeyManager::ToDword( const char* str )
{
	DWORD value = 0;
	memcpy( &value, str, sizeof( DWORD ) );
	return value;
}

std::string RegistryKeyManager::ToString( const DWORD dword )
{
	return std::string( (char*) &dword );
}

std::string RegistryKeyManager::ToBackslash( std::string input )
{
	for ( auto& c : input )
		if ( c == '/' )
			c = '\\';
	return input;
}

/// <summary> Used with hkeyMap </summary>
#define HkeyEntry(h) { #h, h }
std::map<string, HKEY> hkeyMap(
{
	HkeyEntry( HKEY_CLASSES_ROOT ),
	HkeyEntry( HKEY_CURRENT_USER ),
	HkeyEntry( HKEY_LOCAL_MACHINE ),
	HkeyEntry( HKEY_USERS ),
	HkeyEntry( HKEY_CURRENT_CONFIG )
} );

HKEY RegistryKeyManager::ToHkey( std::string hkeystr )
{
	// Convert string to upper case.
	transform( hkeystr.begin(), hkeystr.end(), hkeystr.begin(), toupper );
	return hkeyMap[ hkeystr ];
}

#pragma endregion

#pragma region Ctor (parsing command line arguments)

RegistryKeyManager::RegistryKeyManager( std::string path, const DWORD value )
{
	path = ToBackslash( path );
	std::string name;

	// The following lines separate the key name from root.
	auto firstSlash = path.find_first_of( '\\' );
	if ( firstSlash < std::string::npos - 1 )
	{
		name.insert( name.begin(), path.begin() + firstSlash + 1, path.end() );
		path.erase( firstSlash );
	}

	m_RootKey = ToHkey( path );
	m_Name = name;
	WriteValue( value );
}

#pragma endregion

	


