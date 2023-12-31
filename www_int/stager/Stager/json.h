
#include <Windows.h>


	typedef enum nx_json_type {
		NX_JSON_NULL,    // this is null value
		NX_JSON_OBJECT,  // this is an object; properties can be found in child nodes
		NX_JSON_ARRAY,   // this is an array; items can be found in child nodes
		NX_JSON_STRING,  // this is a string; value can be found in text_value field
		NX_JSON_INTEGER, // this is an integer; value can be found in int_value field
		NX_JSON_DOUBLE,  // this is a double; value can be found in dbl_value field
		NX_JSON_BOOL     // this is a boolean; value can be found in int_value field
	} nx_json_type;

	typedef struct nx_json {
		nx_json_type type;       // type of json node, see above
		const char* key;         // key of the property; for object's children only
		const char* text_value;  // text value of STRING node
		long int_value;     // the value of INTEGER or BOOL node
		double dbl_value;        // the value of DOUBLE node
		int length;              // number of children of OBJECT or ARRAY
		struct nx_json* child;   // points to first child
		struct nx_json* next;    // points to next child
		struct nx_json* last_child;
	} nx_json;

	typedef int(*nx_json_unicode_encoder)(unsigned int codepoint, char* p, char** endp);

	extern nx_json_unicode_encoder nx_json_unicode_to_utf8;

	const nx_json* nx_json_parse(char* text, nx_json_unicode_encoder encoder);
	const nx_json* nx_json_parse_utf8(char* text);
	void nx_json_free(const nx_json* js);
	const nx_json* nx_json_get(const nx_json* json, const char* key); // get object's property by key
	const nx_json* nx_json_item(const nx_json* json, int idx); // get array element by index


