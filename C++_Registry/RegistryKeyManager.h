#pragma once
#include <string>
#include <vector>
#include <map>
#include <Windows.h>

/// <summary> This encapsulates all the code necessary to manage a registry in one class,
/// so that it could be included easily in your code</summary>
class RegistryKeyManager
{
#pragma region key properties
public:

	std::string GetName() const { return m_Name; }
	void SetName( const std::string& name ) { m_Name = name; }

protected:

	/// <summary> Stores key name </summary>
	std::string m_Name;

	/// <summary> Stores key location </summary>
	std::vector<std::string> m_Root;

#pragma endregion

#pragma region Accessing the registry
protected:

	/// <summary> Opens key for reading / writing </summary>
	/// <remarks> Displays appropriate error messages in case of open failure. </remarks>
	LSTATUS Open();

	/// <summary> Closes Key after reading / writing </summary>
	/// <remarks> Displays appropriate error messages in case of open failure. </remarks>
	LSTATUS Close();

	/// <summary>Sets root key (e.g. HKEY_CURRENT_USER)</summary>
	HKEY GetRootKey() const { return m_RootKey; }

protected:
	HKEY m_Key;
	HKEY m_RootKey;

#pragma endregion

#pragma region Reading and writing key values
public:

	/// <summary>Reads a DWORD value from the registry</summary>
	/// <remarks>Opens and closes the key upon call/return<remarks>
	DWORD ReadDwordValue();

	/// <summary>Writes a DWORD value to the registry</summary>
	/// <remarks>Opens and closes the key upon call/return<remarks>
	BOOL WriteValue( const DWORD value );

#pragma endregion

#pragma region Value conversions
public:

	/// <summary>Converts a C-string to a DWORD</summary>
	static DWORD ToDword( const char*  );

	/// <summary>Converts a DWORD to an std-string</summary>
	static std::string ToString( const DWORD );

	/// <summary>Converts a string representation of a registry root (e.g. "hkey_current_user")
	/// to an actual HKEY define (e.g. HKEY_CURRENT_USER).</summary>
	static HKEY ToHkey( std::string );

	/// <summary>Converts all instances of forward slashes to back slashes.</summary>
	static std::string ToBackslash( std::string input );

private:

#pragma endregion

#pragma region Ctor
public:

	/// <summary> Default constructor - Root and Key names must be set manually.</summary>
	RegistryKeyManager() {}

	/// <summary>Constructor with key root and name </summary>
	/// <param name="rootKey"> default root where the key is stored, such as HKEY_CURRENT_USER. </param>
	RegistryKeyManager( const HKEY rootKey, const std::string name ) { m_RootKey = rootKey; m_Name = name; }

	/// <summary>Constructor with key root, name and a value</summary>
	/// <param name="rootKey"> default root where the key is stored, such as HKEY_CURRENT_USER. </param>
	RegistryKeyManager( const HKEY rootKey, const std::string name, const DWORD value ) : RegistryKeyManager( rootKey, name ) { WriteValue( value ); }

	/// <summary>Constructor with key path (root/name) and a value</summary>
	RegistryKeyManager( std::string fullPath, const DWORD value );

#pragma endregion

}; // RegistryKeyManager

/// <summary> This defines which command line argument the users needs to enter before the the root.</summary>
#define CLARG_PATH "-k"

/// <summary> This defines which command line argument the users needs to enter before the the value.</summary>
#define CLARG_KEYVAL "-v"
