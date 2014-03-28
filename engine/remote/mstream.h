#pragma once
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <list>
#include <set>
#include <map>
/**
 * 可以发送容器类型的消息体
 * 对于消息体中的指针 外部不可以存储
 * */
#define SERIALIZE_CMD() \
	virtual void __to_msg__record__(unsigned char tag,cmd::Stream *ss)	
#define CMD_SERIALIZE()\
	virtual void __to_msg__record__(unsigned char tag,cmd::Stream *ss)
#define CALL_CMD_SERIALIZE(__parent__class__)\
    __parent__class__::__to_msg__record__(tag,ss)
#define BIND(value)\
	switch(tag)\
	{\
		case cmd::GET:\
		{\
			getRecord(value,ss);	\
		}break;\
		case cmd::ADD:\
		{\
			addRecord(value,ss);\
		}break;\
		case cmd::CLEAR:\
		{\
			clearRecord(value);\
		}break;\
	}
#define BIND_CALLBACK(value,callback)\
	switch(tag)\
	{\
		case cmd::GET:\
		{\
			getRecord(value,ss);	\
			callback(cmd::GET);\
		}break;\
		case cmd::ADD:\
		{\
			addRecord(value,ss);\
			callback(cmd::ADD);\
		}break;\
		case cmd::CLEAR:\
		{\
			clearRecord(value);\
		}break;\
	}

#define BIND_SIZE(value,size)\
	switch(tag)\
	{\
		case cmd::GET:\
		{\
			getRecord(value,size,ss);	\
		}break;\
		case cmd::ADD:\
		{\
			addRecord(value,size,ss);\
		}break;\
		case cmd::CLEAR:\
		{\
			clearRecord(value,size);\
		}break;\
	}

namespace cmd{
	typedef unsigned short BodySize_t;
	struct Stream{
		virtual bool append(const void *content,unsigned int size) = 0;
		virtual bool pick(void *content,unsigned int size) = 0;
		virtual void reset() = 0;
		virtual ~Stream(){}
	};
	struct BinaryStream:public Stream{
		virtual void set(void *content,int size)
		{
			_offset = 0;
			if (!size) return;
			contents.resize(size);
			memcpy(&contents[_offset],content,size);
		}
		BinaryStream(){
			_offset = 0;
			_size = 0;
		}
		virtual ~BinaryStream(){}
		virtual bool append(const void *content,unsigned int size)
		{
			if (size == 0) return false;
			if (contents.size() - _offset < size)
			{
				contents.resize(_offset + size);
			}
			memcpy(&contents[_offset],content,size);
			_offset += size;
			_size = _offset;
			return true;
		}
		virtual bool pick(void *content,unsigned int size)
		{
			if (contents.size() - _offset < size)
			{
				return false;
			}
			memcpy(content,&contents[_offset],size);
			_offset += size;
			return true;
		}
		virtual void reset(){_offset = 0;}
		std::vector<unsigned char> contents;
		virtual unsigned int size(){return _size;}
		virtual void * content(){return &contents[0];}
	private:
		int _offset;
		int _size;
	};
	struct FileStream:public Stream{
		FileStream()
		{
			_file = NULL;
		}
		~FileStream(){}
		FileStream(FILE *file)
		{
			this->_file = file;
			get_file_size();
		}
		bool good(){return true;}
		void read(char *data,size_t size)
		{
			pick(data,size);
		}
		void write(char *data,size_t size)
		{
			append(data,size);
		}
		void open(const char *filename,const char *tag)
		{
			_file = fopen(filename,tag);
			get_file_size();
		}
		void close()
		{
			if (_file) fclose(_file);
			_file = NULL;
		}
		void flush(){}
		virtual bool append(const void *content,unsigned int size)
		{
			if (!_file) return false;
			fwrite(content,size,1,_file);
			_offset += size;
			_file_size = _offset;
			return true;
		}
		virtual bool pick(void *content,unsigned int size)
		{
			if (!_file) return false;
			if (_file_size - _offset < (int)size)
			{
				return false;
			}
			fread(content,size,1,_file);
			_offset += size;
			return true;
		}
		virtual void reset()
		{
			_offset = 0;
		}
		int get_file_size()
		{
			long curpos = 0;
			curpos = ftell(_file);
			fseek(_file, 0L, SEEK_END);
			_file_size = ftell(_file);
			fseek(_file, curpos, SEEK_SET);
			return _file_size;
		}
	private:
		FILE *_file;
		int _file_size;
		int _offset;
	};
	const int GET = 0;
	const int ADD = 1;
	const int CLEAR = 2;
#define DEC_OBJECT_RECORD(container,insert_func)\
	template<typename T>\
	void addRecord(typename container<T> &objects,Stream *ss)\
	{\
		int size = objects.size();\
		ss->append(&size,sizeof(int));\
		for (typename container<T>::iterator iter = objects.begin(); iter != objects.end();++iter)\
		{\
			iter->__to_msg__record__(ADD,ss);\
		}\
	}\
	template<typename T>\
	void getRecord(typename container<T> &objects,Stream *ss)\
	{\
		int size;\
		if (ss->pick(&size,sizeof(int)))\
		{\
			while ( size-- > 0)\
			{\
				T value;\
				value.__to_msg__record__(GET,ss);\
				objects.insert_func(value);\
			}\
		}	\
	}\
	template<typename T>\
	void clearRecord(typename container<T> &objects)\
	{\
		objects.clear();\
	}\

#define DEC_TYPE_RECORD(container,insert_func,type)\
	void addRecord(container<type> &objects,Stream *ss)\
	{\
		int size = objects.size();\
		ss->append(&size,sizeof(int));\
		for (container<type>::iterator iter = objects.begin(); iter != objects.end();++iter)\
		{\
			type value = *iter;\
			addRecord(value,ss);\
		}\
	}\
	void getRecord(container<type> &objects,Stream *ss)\
	{\
		int size;\
		if (ss->pick(&size,sizeof(int)))\
		{\
			while ( size-- > 0)\
			{\
				type value;\
				getRecord(value,ss);\
				objects.insert_func(value);\
			}\
		}	\
	}\
	void clearRecord(container<type> &objects)\
	{\
		objects.clear();\
	}
#define DEC_TYPE(__type__)\
	void addRecord(__type__& value,Stream *ss) {ss->append(&value,sizeof(__type__));}\
	void getRecord(__type__ &value,Stream *ss) {ss->pick(&value,sizeof(__type__));}\
	void clearRecord(__type__ &value) {value =0;}
	class Object{
	public:
		DEC_OBJECT_RECORD(std::vector,push_back);
		DEC_OBJECT_RECORD(std::list,push_back);
		DEC_OBJECT_RECORD(std::set,insert);
		DEC_TYPE_RECORD(std::vector,push_back,int);
		DEC_TYPE_RECORD(std::list,push_back,int);
		DEC_TYPE_RECORD(std::set,insert,int);
		//DEC_TYPE_RECORD(std::vector,push_back,char);
		DEC_TYPE(int);
		DEC_TYPE(float);
		DEC_TYPE(char);
		DEC_TYPE(unsigned int);
		DEC_TYPE(unsigned char);
		template <class T>
		void addRecord(T& value,Stream *ss)
		{
			value.__to_msg__record__(ADD,ss);
		}
		template<class T>
		void getRecord(T &value,Stream *ss)
		{
			value.__to_msg__record__(GET,ss);
		}

		template<class T>
		void clearRecord(T &value)
		{
		}

		void addRecord(std::string &value,Stream* ss)
		{
			BodySize_t size = value.size();
			ss->append(&size,sizeof(BodySize_t));
			ss->append((void*)value.c_str(),size);
		}
		void addRecord(const void *value,int size,Stream *ss)
		{
			ss->append((void*)value,size);
		}
		void getRecord(void *value,int size,Stream *ss)
		{
			ss->pick(value,size);
		}
		
		void clearRecord(void * value,int size)
		{
			memset(value,0,size);
		}
		void addRecord(const std::basic_string<char> &value,Stream *ss)
		{
			BodySize_t size = value.size();
			ss->append(&size,sizeof(BodySize_t));
			ss->append((void*)value.c_str(),size);
		}
		void getRecord(std::basic_string<char> &value,Stream *ss)
		{
			BodySize_t size = 0;
			if (ss->pick(&size,sizeof(BodySize_t)))
			{
				if (size > 0)
				{
					char *str = new char [size +1];
					memset(str,0,size+1);
					if (ss->pick(str,size))
						value = str;
					delete[] str;
				}
			}
		}
		void addRecord(std::vector<char> &value,Stream *ss)
		{
			BodySize_t size = value.size();
			ss->append(&size,sizeof(BodySize_t));
			ss->append((void*)&value[0],value.size());
		}
		void getRecord(std::vector<char> &value,Stream *ss)
		{
			BodySize_t size = 0;
			if (ss->pick(&size,sizeof(BodySize_t)))
			{
				if (size > 0)
				{
					value.resize(size);
					ss->pick(&value[0],size);
				}
			}
		}

		void clearRecord(std::string &value)
		{
			value = "";
		}
		void addRecord(std::vector<std::string> &value,Stream *ss)
		{
			BodySize_t size = value.size();
			ss->append(&size,sizeof(BodySize_t));
			for (std::vector<std::string>::iterator iter = value.begin(); iter != value.end();++iter)
			{
				addRecord(*iter,ss);
			}
		}
		void getRecord(std::vector<std::string> &value,Stream *ss)
		{
			BodySize_t size = 0;
			if (ss->pick(&size,sizeof(BodySize_t)))
			{
				while (size -- >0)
				{
					std::string str;
					getRecord(str,ss);
					value.push_back(str);
				}
			}	
		}
		void clearRecord(std::vector<std::string> &value)
		{
			value.clear();
		}

		template<class KEY,class VALUE>
		void addRecord(typename std::map<KEY,VALUE> &value,Stream *ss)
		{
			BodySize_t size = value.size();
			ss->append(&size,sizeof(BodySize_t));
			for (typename std::map<KEY,VALUE>::iterator iter = value.begin(); iter != value.end();++iter)
			{
				addRecord(iter->first,ss);
				addRecord(iter->second,ss);
			}
		}
		template<class KEY,class VALUE>
		void getRecord(typename std::map<KEY,VALUE> &values,Stream *ss)
		{
			BodySize_t size = 0;
			if (ss->pick(&size,sizeof(BodySize_t)))
			{
				while (size -- >0)
				{
					KEY key;
					VALUE value;
					getRecord(key,ss);
					getRecord(value,ss);	
					values[key] = value;
				}
			}	
		}
		template<class KEY,class VALUE>
		void clearRecord(typename std::map<KEY,VALUE> &values)
		{
			values.clear();
		}

	public:
		template<typename STREAM>
		STREAM toStream()
		{
			STREAM ss;
			__to_msg__record__(ADD,ss);
			ss.reset();
			return ss;
		}
		void fromStream(void * content,int size)
		{
			if (size == 0) return;
			BinaryStream ss;
			ss.set(content,size);
			fromStream(ss);
		}
		template<typename STREAM>
		void fromStream(STREAM& ss)
		{
			ss.reset();
			__to_msg__record__(GET,&ss);
			ss.reset();
		}
		void clearContent()
		{
			//Stream ss;	
			//__to_msg__record__(CLEAR,ss);
		}
		virtual ~Object(){}
	public:
		virtual void __to_msg__record__(unsigned char tag,Stream* ss)
    		{
    		};
	};
};


/**
 * 消息体,携带了消息唯一标示
 */
class CmdObject:public cmd::Object{
public:
	unsigned int __msg__id__; // 消息号
	/**
	 * 处理内容
	 * \param cmd 内容
	 * \param cmdLen 内容长度
	 */
	virtual void parsecmd(void *cmd,int cmdLen)
	{
		if ((unsigned int)cmdLen < sizeof(int)) return;	
		//TODO __msg__id__ = *(int*) cmd;
		fromStream(((char*)cmd + sizeof(int)),cmdLen - sizeof(int));	
	}
	virtual void parsecmdandid(void *cmd,int cmdLen)
	{
		if (cmdLen < (int)sizeof(int)) return;	
		__msg__id__ = *(int*) cmd;
		fromStream(((char*)cmd + sizeof(int)),cmdLen - sizeof(int));
	}
	/**
	 * 清除消息结构体内容
	 * 消息结构体 中的内容是暂存的
	 */
	virtual void clear()
	{
		clearContent();
	}
	/**
	 * 将消息转化为字符串
	 */
	template<typename STREAM>
	STREAM toStream()
	{
		STREAM ss;
		ss.append(&__msg__id__,sizeof(__msg__id__));
		__to_msg__record__(cmd::ADD,&ss);
		ss.reset();
		return ss;	
	}
	void write(cmd::Stream* ss)
	{
		ss->append(&__msg__id__,sizeof(__msg__id__));
		__to_msg__record__(cmd::ADD,ss);
		ss->reset();
	}
	void read(cmd::Stream *ss)
	{
		ss->pick(&__msg__id__,sizeof(__msg__id__));
		__to_msg__record__(cmd::GET,ss);
	}
	/**
	 * 使用唯一id 标示消息号
	 * \param id 唯一
	 */
	CmdObject(unsigned int id):__msg__id__(id){}
	CmdObject(){}
	virtual ~CmdObject(){}
public:
	virtual void __to_msg__record__(unsigned char tag,cmd::Stream* ss){};
};
