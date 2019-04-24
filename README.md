# C-RegistryWriter
C++ Application To Write Data Into HKEY_CURRENT_USER

A DWORD is 4 bytes, so the value for RegistryWriter to send is currently limited to 4 characters (i.e. test1234 is not technically possible with a DWORD)

This project includes a RegistryKeyManager class that takes care of writing/reading to a registry file. Using a dedicated class makes expanding your registry functionality in the future easier.

# To use
In your code, using the class is simple. The simplest example would be something like
RegistryKeyManager r( "HKEY_CURRENT_USER/Software/x/y", 1234 );
Which would create a key in HKEY_CURRENT_USER/Software/x/y with a DWORD value of 1234.

You could then read the value of and modify the key with calls like
DWORD currentValue = r.ReadDwordValue();
r.WriteValue( 1234 );

A small helper method is also included in case you'd like to encode strings in the dword (since in your example you have used an ASCII string as the value. The method is RegistryKeyManager::ToDword(). You just need to call it around the value:
r.WriteValue( RegistryKeyManager::ToDword( "TEST" )); // Stores the bytes T, E, S, T in the DWORD value.
