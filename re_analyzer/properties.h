#define property_get_set_implemented(_type, name) __declspec(property(get=get_##name, put=set_##name)) _type name;\
	_type get_##name(void) const{ return _##name; }\
	void set_##name(_type val){ _##name = val; }

#define property_get_implemented(_type, name) __declspec(property(get=get_##name)) _type name;\
	inline _type get_##name(void) const{ return _##name; }

#define property_get_implemented_not_const(_type, name) __declspec(property(get=get_##name)) _type name;\
	inline _type get_##name(void) { return _##name; }

#define property_set_implemented(_type, name) __declspec(property(put=set_##name)) _type name;\
	void set_##name(_type val){ _##name = val; }

#define property_get_set(_type, name) __declspec(property(get=get_##name, put=set_##name)) _type name;\
	_type get_##name(void) const;\
	void set_##name(const _type& val);

#define property_get(_type, name) __declspec(property(get=get_##name)) _type name;\
	_type get_##name(void) const;

#define property_set(_type, name) __declspec(property(put=set_##name)) _type name;\
	void set_##name(const _type& val);

#define property_get_not_const(_type, name) __declspec(property(get=get_##name)) _type name;\
	_type get_##name(void);