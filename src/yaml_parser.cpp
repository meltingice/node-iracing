// Copyright (c) 2011 iRacing.com Motorsport Simulations, LLC.
// All rights reserved.

#include <stdio.h>
#include <string.h>

enum yaml_state {
	space,
	key,
	keysep,
	value,
	newline
};

// super simple YAML parser
bool parseYaml(const char *data, const char* path, const char **val, int *len)
{
	if(data && path && val && len)
	{
		// make sure we set this to something
		*val = NULL;
		*len = 0;

		int depth = 0;
		yaml_state state = space;

		const char *keystr = NULL;
		int keylen = 0;

		const char *valuestr = NULL;
		int valuelen = 0;

		const char *pathptr = path;
		int pathdepth = 0;

		while(*data)
		{
			switch(*data)
			{
			case ' ':
			case '-':
				if(state == newline)
					state = space;
				if(state == space)
					depth++;
				else if(state == key)
					keylen++;
				else if(state == value)
					valuelen++;
				break;
			case ':':
				if(state == key)
				{
					state = keysep;
					keylen++;
				}
				else if(state == keysep)
				{
					state = value;
					valuestr = data;
				}
				else if(state == value)
					valuelen++;
				break;
			case '\n':
			case '\r':
				if(state != newline)
				{
					if(depth < pathdepth)
					{
						return false;
					}
					else if(keylen && 0 == strncmp(keystr, pathptr, keylen))
					{
						bool found = true;
						//do we need to test the value?
						if(*(pathptr+keylen) == '{')
						{
							//search for closing brace
							int pathvaluelen = keylen + 1; 
							while(*(pathptr+pathvaluelen) && *(pathptr+pathvaluelen) != '}')
								pathvaluelen++; 

							if(valuelen == pathvaluelen - (keylen+1) && 0 == strncmp(valuestr, (pathptr+keylen+1), valuelen))
								pathptr += valuelen + 2;
							else
								found = false;
						}

						if(found)
						{
							pathptr += keylen;
							pathdepth = depth;

							if(*pathptr == '\0')
							{
								*val = valuestr;
								*len = valuelen;
								return true;
							}
						}
					}

					depth = 0;
					keylen = 0;
					valuelen = 0;
				}
				state = newline;
				break;
			default:
				if(state == space || state == newline)
				{
					state = key;
					keystr = data;
					keylen = 0; //redundant?
				}
				else if(state == keysep)
				{
					state = value;
					valuestr = data;
					valuelen = 0; //redundant?
				}
				if(state == key)
					keylen++;
				if(state == value)
					valuelen++;
				break;
			}

			// important, increment our pointer
			data++;
		}

	}
	return false;
}

