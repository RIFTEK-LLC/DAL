#include "../dal_struct.h"
#include "dal_deserializer.h"


dalResult_e	_dal_deserialize_recurse(uint8_t nesting, dalDeserializer_t* deser, dal_t* node)
{
	//Check the nesting of recursion, it must be limited, otherwise the wrong data may loop
	//calls
	if (nesting > 32)					return DAL_FORMAT_ERR;
	if (node == nullptr)				return DAL_FORMAT_ERR;

	dalDeserializerValue_t	value;
	dalResult_e				result	= deser->read(deser->context, &deser->buf, &deser->available, &value);
	if (result != DAL_OK)	return result;
	dalStr_t				valStr;
	dalBlob_t				valBlob;
	dal_t*					newNode;
	dal_t*					arrNode;
	uint32_t				count;

	switch (value.type)
	{
		default:
		case DVT_UNKN:		return DAL_FORMAT_ERR;

		case DVT_BOOL:
			*node			= static_cast<bool>(value.as_bool);
			break;

		case DVT_UINT:
			*node			= value.as_uint;
			break;

		case DVT_INT:
			*node			= value.as_int;
			break;

		case DVT_DBL:
			*node			= value.as_dbl;
			break;

		case DVT_STR:
			valStr.data		= value.as_str;
			valStr.size		= value.size;
			*node			= valStr;
			break;

		case DVT_BLOB:
			valBlob.data	= value.as_blob;
			valBlob.size	= value.size;
			*node			= valBlob;
			break;

		case DVT_ARRAY:
			arrNode			= node->convert_to_array(value.size);
			if (arrNode == nullptr)		return DAL_MEM_ERR;
			while (value.size-- > 0)
			{
				result		= _dal_deserialize_recurse(nesting + 1, deser, arrNode);
				if (result != DAL_OK)		return result;
				arrNode++;
			}
			break;

		case DVT_OBJECT:
			count			= value.size;
			while (count-- > 0)
			{
				//The first element of an object should be its key
				//-so that we read the next element and it must be a string representing the key
				result		= deser->read(deser->context, &deser->buf, &deser->available, &value);
				if (result != DAL_OK)		return result;
				if (value.type != DVT_STR)	return DAL_FORMAT_ERR;
				newNode		= node->create_child();
				valStr.data	= value.as_str;
				valStr.size	= value.size;
				newNode->rename(&valStr);
				result		= _dal_deserialize_recurse(nesting + 1, deser, newNode);
				if (result != DAL_OK)		return result;
			}
			break;
	}

	return result;
}
