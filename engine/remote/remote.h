#pragma once
#include "mstream.h"
/**
 * 模板元真的狠强大 我只是个未入门的 就简单实用并做了协议的参数化
 */
namespace remote{
	template<typename A>
	struct is_ref { static const bool value = false; };
	
	template<typename A>
	struct is_ref<A&> { static const bool value = true; };

	template<typename A>
	struct ischars { static const bool value = false; };
	template<>
	struct ischars<const char*> { static const bool value = true; };

	template<typename A>
	struct charstostring{
		typedef A type;
	};
	template<>
	struct charstostring<const char*> { typedef std::string type; };

	template<typename A>
	struct ispointer { static const bool value = false; };
	template<typename A>
	struct ispointer<A*> { static const bool value = true; };

	template<typename A>
	struct istype{static const bool value = false;};
	template<>
	struct istype<int>{static const bool value = true;};
	template<>
	struct istype<float>{static const bool value = true;};
	template<>
	struct istype<long>{static const bool value = true;};
	template<>
	struct istype<double>{static const bool value = true;};
	template<>
	struct istype<unsigned int>{static const bool value = true;};
	template<>
	struct istype<unsigned long>{static const bool value = true;};
	template<>
	struct istype<char>{static const bool value = true;};
	template<>
	struct istype<short>{static const bool value = true;};
	template<>
	struct istype<unsigned char>{static const bool value = true;};
	template<>
	struct istype<unsigned short>{static const bool value = true;};

	template<typename A>
	struct iscmd{
		static const bool value = false;
	};
	template<>
	struct iscmd<cmd::Object>{
		static const bool value = true;
	};
	
	template<typename A>
	struct isstring{
		static const bool value = false;
	};
	template<>
	struct isstring<std::string>{
		static const bool value = true;
	};

	template<bool C, typename A, typename B> struct if_ {};
	template<typename A, typename B>		struct if_<true, A, B> { typedef A type; };
	template<typename A, typename B>		struct if_<false, A, B> { typedef B type; };

	template<typename A>
	struct remove_const { typedef  A type; };
	template<typename A>
	struct remove_const<const A> { typedef  A type; };

	template<typename A>
	struct remove_ref { typedef  A type; };
	template<typename A>
	struct remove_ref< A & > { typedef  A type; };
	
	template<typename A>
	struct remove_pointer { typedef A type; };
	template<typename A>
	struct remove_pointer< A * > { typedef  A type; };

	template<typename TYPE>
	struct base_type{
		static bool toStream(TYPE &type,cmd::Stream *ss)
		{
			ss->append(&type,sizeof(TYPE));
			return true;
		}
		static bool parseStream(TYPE &type,cmd::Stream *ss)
		{
			ss->pick(&type,sizeof(TYPE));
			return true;
		}
	};
	template<typename PACK>
	struct pack_type{
		static bool toStream(PACK* & type,cmd::Stream *ss)
		{
			ss->append(type,sizeof(PACK));
			return true;
		}
		static bool parseStream(PACK* & type,cmd::Stream *ss)
		{
			ss->pick(type,sizeof(PACK));
			return true;
		}
	};
	template<typename OBJECT>
	struct object_type{
		static bool toStream(OBJECT &object,cmd::Stream *ss)
		{
			object.__to_msg__record__(cmd::ADD,ss);
			return true;
		}
		static bool parseStream(OBJECT &type,cmd::Stream *ss)
		{
			type.__to_msg__record__(cmd::GET,ss);
			return true;
		}
	};
	template<typename OBJECT>
	struct string_type{
		static bool toStream(OBJECT &value,cmd::Stream *ss)
		{
			cmd::BodySize_t size = value.size();
			ss->append(&size,sizeof(cmd::BodySize_t));
			ss->append((void*)value.c_str(),size);
			return true;
		}
		static bool parseStream(OBJECT &value,cmd::Stream *ss)
		{
			cmd::BodySize_t size = 0;
			if (ss->pick(&size,sizeof(cmd::BodySize_t)))
			{
				if (size > 0)
				{
					char *str = new char [size +1];
					memset(str,0,size+1);
					if (ss->pick(str,size))
						value = str;
					delete[] str;
				}
				return true;
			}
			return false;
		}
	};
	template<typename OBJECT>
	bool toStream(OBJECT &object,cmd::Stream *ss)
	{
		if_<isstring<OBJECT>::value,
			string_type<OBJECT>,
			typename if_<istype<OBJECT>::value,
			base_type<OBJECT>,
			typename if_<iscmd<OBJECT>::value,
				object_type<OBJECT>,
					typename if_<ispointer<OBJECT>::value,
					pack_type<typename remove_pointer<OBJECT>::type>,
					base_type<OBJECT> > ::type
				>::type 
			>::type>::type::toStream(object,ss);
		return true;
	}
	template<typename OBJECT>
	bool parseStream(OBJECT &object,cmd::Stream *ss)
	{
		if_<isstring<OBJECT>::value,
			string_type<OBJECT>,
			typename if_<istype<OBJECT>::value,
			base_type<OBJECT>,
			typename if_<iscmd<OBJECT>::value,
				object_type<OBJECT>,
					typename if_<ispointer<OBJECT>::value,
					pack_type<typename remove_pointer<OBJECT>::type>,base_type<OBJECT> > ::type
				>::type 
			>::type>::type::parseStream(object,ss);
		return true;
	}
	template<unsigned char x,unsigned char y>
	struct Head{
		enum{
			X = x,
			Y = y,
		};
	};
	/**
	 * 可使用模板进行内存使用上的优化
	 */
	struct function{
		virtual void call(const char *content,unsigned int len){};
		virtual void call(void *object,const char *content,unsigned int len){};
		std::string name;
		enum INDEXTYPE{
			STRING,
			USHORT,
			XY,
		} indexType;
		unsigned short index;
		unsigned char x;
		unsigned char y;
		void set(const std::string &name)
		{
			this->name = name;
			indexType = STRING;
		}
		void set(unsigned short index)
		{
			this->index = index;
			indexType = USHORT;
		}
		void set(unsigned char x,unsigned char y)
		{
			this->x = x;
			this->y = y;
			indexType = XY;
		}
		template<typename HEAD>
		void set(HEAD head)
		{
			this->x = HEAD::X;
			this->y = HEAD::Y;
			indexType = XY;
		}
		void parseIndex(cmd::Stream *ss)
		{
			switch(indexType)
			{
				case STRING:
				{
					parseStream(name,ss);
				}break;
				case XY:
				{
					parseStream(x,ss);
					parseStream(y,ss);
				}break;
				case USHORT:
				{
					parseStream(index,ss);
				}break;
			}
		}
		void toIndex(cmd::Stream *ss)
		{
			switch(indexType)
			{
				case STRING:
				{
					toStream(name,ss);
				}break;
				case XY:
				{
					toStream(x,ss);
					toStream(y,ss);
				}break;
				case USHORT:
				{
					toStream(index,ss);
				}break;
			}
		}
		virtual ~function(){}
		function()
		{
			x = y = 0;
			index = 0;
		}
	};

#define PASRE_ARG(T,obj) \
	typename remove_pointer<typename remove_const<typename remove_ref<typename charstostring<T>::type>::type>::type >::type obj;	\
	parseStream(obj,&ss);

#define BEGIN_PARSE \
	cmd::BinaryStream ss;\
	ss.set((void*)content,len);\
	parseIndex(&ss);

#define GF_CALL (*func)
#define OF_CALL (((T*)object)->*func)

#define BEGIN_STREAM \
	cmd::BinaryStream ss;\
	toIndex(&ss);

#define TO_STREAM(arg1) \
	toStream(arg1,&ss);

#define END_STREAM\
	std::string temp;\
	temp.resize(ss.size());\
	memcpy(&temp[0],ss.content(),ss.size());\
	return temp;
	template<typename T>
	std::string getClass(void *cmd,T len)
	{
		cmd::BinaryStream ss;
		std::string name;
		ss.set(cmd,len);
		remote::parseStream(name,&ss);
		std::string className;
		for (unsigned int index = 0; index < name.size();++index) 
		{if (name[index] == ':') break;className.push_back(name[index]);}
		return className;
	}
	template<typename T>
	std::string getClass(const T& temp)
	{
		cmd::BinaryStream ss;
		std::string name;
		ss.set((void*)&temp[0],temp.size());
		remote::parseStream(name,&ss);
		std::string className;
		for (unsigned int index = 0; index < name.size();++index) 
		{if (name[index] == ':') break;className.push_back(name[index]);}
		return className;
	}

	template<typename RVAL,typename T1=void,typename T2=void,typename T3=void,typename T4=void,typename T5=void,typename T6=void>
	struct message:public function{
		typedef RVAL (*FUNC)(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			PASRE_ARG(T2,t2);
			PASRE_ARG(T3,t3);
			PASRE_ARG(T4,t4);
			PASRE_ARG(T5,t5);
			PASRE_ARG(T6,t6);

			GF_CALL(t1,t2,t3,t4,t5,t6);
		}
		std::string build(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			TO_STREAM(arg2);
			TO_STREAM(arg3);
			TO_STREAM(arg4);
			TO_STREAM(arg5);
			TO_STREAM(arg6);
			END_STREAM
		}
	};
	template<typename RVAL,typename T,typename T1=void,typename T2=void,typename T3=void,typename T4=void,typename T5=void,typename T6=void>
	struct omessage:public function{
		typedef RVAL (T::*FUNC)(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(void *object,const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			PASRE_ARG(T2,t2);
			PASRE_ARG(T3,t3);
			PASRE_ARG(T4,t4);
			PASRE_ARG(T5,t5);
			PASRE_ARG(T6,t6);
			OF_CALL (t1,t2,t3,t4,t5,t6);
		}
		std::string build(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5,T6 arg6)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			TO_STREAM(arg2);
			TO_STREAM(arg3);
			TO_STREAM(arg4);
			TO_STREAM(arg5);
			TO_STREAM(arg6);
			END_STREAM
		}
	};

	template<typename RVAL,typename T1,typename T2,typename T3,typename T4,typename T5>
	struct message<RVAL,T1,T2,T3,T4,T5>:public function{
		typedef RVAL (*FUNC)(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			PASRE_ARG(T2,t2);
			PASRE_ARG(T3,t3);
			PASRE_ARG(T4,t4);
			PASRE_ARG(T5,t5);

			GF_CALL(t1,t2,t3,t4,t5);
		}
		std::string build(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			TO_STREAM(arg2);
			TO_STREAM(arg3);
			TO_STREAM(arg4);
			TO_STREAM(arg5);
			END_STREAM
		}
	};

	template<typename RVAL,typename T,typename T1,typename T2,typename T3,typename T4,typename T5>
	struct omessage<RVAL,T,T1,T2,T3,T4,T5>:public function{
		typedef RVAL (T::*FUNC)(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(void *object,const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			PASRE_ARG(T2,t2);
			PASRE_ARG(T3,t3);
			PASRE_ARG(T4,t4);
			PASRE_ARG(T5,t5);
			OF_CALL (t1,t2,t3,t4,t5);
		}
		std::string build(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			TO_STREAM(arg2);
			TO_STREAM(arg3);
			TO_STREAM(arg4);
			TO_STREAM(arg5);
			END_STREAM
		}
	};

	template<typename RVAL,typename T1,typename T2,typename T3,typename T4>
	struct message<RVAL,T1,T2,T3,T4>:public function{
		typedef RVAL (*FUNC)(T1 arg1,T2 arg2,T3 arg3,T4 arg4);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			PASRE_ARG(T2,t2);
			PASRE_ARG(T3,t3);
			PASRE_ARG(T4,t4);
			GF_CALL(t1,t2,t3,t4);
		}
		std::string build(T1 arg1,T2 arg2,T3 arg3,T4 arg4)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			TO_STREAM(arg2);
			TO_STREAM(arg3);
			TO_STREAM(arg4);

			END_STREAM
		}
	};

	template<typename RVAL,typename T,typename T1,typename T2,typename T3,typename T4>
	struct omessage<RVAL,T,T1,T2,T3,T4>:public function{
		typedef RVAL (T::*FUNC)(T1 arg1,T2 arg2,T3 arg3,T4 arg4);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(void *object,const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			PASRE_ARG(T2,t2);
			PASRE_ARG(T3,t3);
			PASRE_ARG(T4,t4);
			OF_CALL (t1,t2,t3,t4);
		}
		std::string build(T1 arg1,T2 arg2,T3 arg3,T4 arg4)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			TO_STREAM(arg2);
			TO_STREAM(arg3);
			TO_STREAM(arg4);
			END_STREAM
		}
	};
	
	template<typename RVAL,typename T1,typename T2,typename T3>
	struct message<RVAL,T1,T2,T3>:public function{
		typedef RVAL (*FUNC)(T1 arg1,T2 arg2,T3 arg3);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			PASRE_ARG(T2,t2);
			PASRE_ARG(T3,t3);
			GF_CALL(t1,t2,t3);
		}
		std::string build(T1 arg1,T2 arg2,T3 arg3)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			TO_STREAM(arg2);
			TO_STREAM(arg3);

			END_STREAM
		}
	};

	template<typename RVAL,typename T,typename T1,typename T2,typename T3>
	struct omessage<RVAL,T,T1,T2,T3>:public function{
		typedef RVAL (T::*FUNC)(T1 arg1,T2 arg2,T3 arg3);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(void *object,const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			PASRE_ARG(T2,t2);
			PASRE_ARG(T3,t3);
			OF_CALL (t1,t2,t3);
		}
		std::string build(T1 arg1,T2 arg2,T3 arg3)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			TO_STREAM(arg2);
			TO_STREAM(arg3);
			END_STREAM
		}
	};

	template<typename RVAL,typename T1,typename T2>
	struct message<RVAL,T1,T2>:public function{
		typedef RVAL (*FUNC)(T1 arg1,T2 arg2);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			PASRE_ARG(T2,t2);
			GF_CALL(t1,t2);
		}
		std::string build(T1 arg1,T2 arg2)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			TO_STREAM(arg2);
			END_STREAM
		}
	};

	template<typename RVAL,typename T,typename T1,typename T2>
	struct omessage<RVAL,T,T1,T2>:public function{
		typedef RVAL (T::*FUNC)(T1 arg1,T2 arg2);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(void *object,const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			PASRE_ARG(T2,t2);
			OF_CALL (t1,t2);
		}
		std::string build(T1 arg1,T2 arg2)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			TO_STREAM(arg2);
			END_STREAM
		}
	};

	template<typename RVAL,typename T1>
	struct message<RVAL,T1>:public function{
		typedef RVAL (*FUNC)(T1 arg1);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			GF_CALL(t1);
		}
		std::string build(T1 arg1)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			END_STREAM
		}
	};
	template<typename RVAL,typename T,typename T1>
	struct omessage<RVAL,T,T1>:public function{
		typedef RVAL (T::*FUNC)(T1 arg1);
		FUNC func;
		void bind(FUNC func)
		{
			this->func = func;
		}
		void call(void *object,const char *content,unsigned int len)
		{
			BEGIN_PARSE
			PASRE_ARG(T1,t1);
			OF_CALL(t1);
		}
		std::string build(T1 arg1)
		{
			BEGIN_STREAM
			TO_STREAM(arg1);
			END_STREAM
		}
	};
	class Messages{
	public:
		std::map<std::string,function*> name_messages;
		typedef std::map<std::string,function*>::iterator NAME_MESSAGES_ITER;
		static Messages&getMe(){
			static Messages msg;
			return msg;
		}
		void addMessage(const std::string & name,function *func)
		{
			name_messages[name] = func;
		}
		function * getMessage(const std::string & name)
		{
			NAME_MESSAGES_ITER iter = name_messages.find(name);
			if (iter != name_messages.end())
			{
				return iter->second;
			}
			return NULL;
		}
		std::vector<function*> id_messages;
		void addMessage(unsigned short index,function *func)
		{
			if (index < id_messages.size()) id_messages.resize(index+1);
			id_messages[index] = func;
		}
		function * getMessage(unsigned short index)
		{
			if (index < id_messages.size())
			{
				return id_messages[index];
			}
			return NULL;
		}

		std::vector<std::vector<function*> > xy_messages;
		void addMessage(unsigned char x,unsigned char y,function *func)
		{
			if (x < xy_messages.size()) xy_messages.resize(x+1);
			if (y < xy_messages[x].size()) xy_messages[x].resize(y + 1);
			xy_messages[x][y] = func;
		}
		function * getMessage(unsigned char x,unsigned char y)
		{
			if (x < xy_messages.size())
			{
				 if(y < xy_messages[x].size())
				 {
					return xy_messages[x][y];
				 }
			}
			return NULL;
		}
	};

	template<typename T>
	bool call(T* object,const char *content,unsigned int size)
	{
		cmd::BinaryStream ss;
		ss.set((void*)content,size);
		std::string name;
		parseStream(name,&ss);
		function * msg = Messages::getMe().getMessage(name.c_str());
		if (msg)
		{
			msg->call(object,content,size);
			return true;
		}
		return false;
	}
	
	template<typename T>
	bool call_id(T* object,const char *content,unsigned int size)
	{
		cmd::BinaryStream ss;
		ss.set((void*)content,size);
		unsigned short index = 0;
		parseStream(index,&ss);
		function * msg = Messages::getMe().getMessage(index);
		if (msg)
		{
			msg->call(object,content,size);
			return true;
		}
		return false;
	}
	
	template<typename T>
	bool call_xy(T* object,const char *content,unsigned int size)
	{
		cmd::BinaryStream ss;
		ss.set((void*)content,size);
		unsigned char x = 0;
		unsigned char y = 0;
		parseStream(x,&ss);
		parseStream(y,&ss);
		function * msg = Messages::getMe().getMessage(x,y);
		if (msg)
		{
			msg->call(object,content,size);
			return true;
		}
		return false;
	}
	// 一个参数
	template<typename INDEX,typename RVAL,typename T1>
	void bind(INDEX name, RVAL (*func)(T1 arg1))
	{
		message<RVAL,typename charstostring<T1>::type> *msg = new message<RVAL,typename charstostring<T1>::type>();
		msg->bind(func);
		func->set(name);
		Messages::getMe().addMessage(name,msg);
	}
	template<typename INDEX,typename RVAL,typename T,typename T1>
	void bind(INDEX name, RVAL (T::*func)(T1 arg1))
	{
		omessage<RVAL,T,T1> *mes = new omessage<RVAL,T,T1>();
		mes->bind(func);
		mes->set(name);
		Messages::getMe().addMessage(name,mes);
	}

	template<typename INDEX,typename RVAL,typename T1>
	std::string build(INDEX func,T1 t1)
	{
		message<RVAL,typename charstostring<T1>::type> msg;
		msg.set(func);	
		return msg.build(t1);
	}

	// 两个参数
	template<typename INDEX,typename RVAL,typename T1,typename T2>
	void bind(INDEX name, RVAL (*func)(T1 arg1,T2 arg2))
	{
		message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type> *msg = new message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type>();
		msg->bind(func);
		func->set(name);
		Messages::getMe().addMessage(name,msg);
	}
	template<typename INDEX,typename RVAL,typename T,typename T1,typename T2>
	void bind(INDEX name, RVAL (T::*func)(T1 arg1,T2 arg2))
	{
		omessage<RVAL,T,T1,T2> *message = new omessage<RVAL,T,T1,T2>();
		message->bind(func);
		message->set(name);
		Messages::getMe().addMessage(name,message);
	}

	template<typename INDEX,typename RVAL,typename T1,typename T2>
	std::string build(INDEX func,T1 t1,T2 t2)
	{
		message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type> msg;
		msg.set(func);
		return msg.build(t1,t2);
	}
	// 三个参数
	template<typename INDEX,typename RVAL,typename T1,typename T2,typename T3>
	void bind(INDEX name, RVAL (*func)(T1 arg1,T2 arg2,T3 arg3))
	{
		message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type,typename charstostring<T3>::type> *msg =
			new message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type,typename charstostring<T3>::type>();
		msg->bind(func);
		func->set(name);
		Messages::getMe().addMessage(name,msg);
	}
	template<typename INDEX,typename RVAL,typename T,typename T1,typename T2,typename T3>
	void bind(INDEX name, RVAL (T::*func)(T1 arg1,T2 arg2,T3 arg3))
	{
		omessage<RVAL,T,T1,T2,T3> *message = new omessage<RVAL,T,T1,T2,T3>();
		message->bind(func);
		message->set(name);
		Messages::getMe().addMessage(name,message);
	}

	template<typename INDEX,typename RVAL,typename T1,typename T2,typename T3>
	std::string build(INDEX func,T1 t1,T2 t2,T3 t3)
	{
		message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type,typename charstostring<T3>::type> msg;
		msg.set(func);
		return msg.build(t1,t2,t3);
	}

	// 四个参数
	template<typename INDEX,typename RVAL,typename T1,typename T2,typename T3,typename T4>
	void bind(INDEX name, RVAL (*func)(T1 arg1,T2 arg2,T3 arg3,T4 arg4))
	{
		message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type,typename charstostring<T3>::type,typename charstostring<T4>::type> *msg =
			new message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type,typename charstostring<T3>::type,typename charstostring<T4>::type>();
		msg->bind(func);
		func->set(name);
		Messages::getMe().addMessage(name,msg);
	}
	template<typename INDEX,typename RVAL,typename T,typename T1,typename T2,typename T3,typename T4>
	void bind(INDEX name, RVAL (T::*func)(T1 arg1,T2 arg2,T3 arg3,T4 arg4))
	{
		omessage<RVAL,T,T1,T2,T3,T4> *message = new omessage<RVAL,T,T1,T2,T3,T4>();
		message->bind(func);
		message->set(name);
		Messages::getMe().addMessage(name,message);
	}

	template<typename INDEX,typename RVAL,typename T1,typename T2,typename T3,typename T4>
	std::string build(INDEX func,T1 t1,T2 t2,T3 t3,T4 t4)
	{
		message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type,typename charstostring<T3>::type,typename charstostring<T4>::type> msg;
		msg.set(func);
		return msg.build(t1,t2,t3,t4);
	}
	// 五个参数
	template<typename INDEX,typename RVAL,typename T1,typename T2,typename T3,typename T4,typename T5>
	void bind(INDEX name, RVAL (*func)(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5))
	{
		message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type,typename charstostring<T3>::type,typename charstostring<T4>::type,typename charstostring<T5>::type> *msg =
			new message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type,typename charstostring<T3>::type,typename charstostring<T4>::type,typename charstostring<T5>::type>();
		msg->bind(func);
		func->set(name);
		Messages::getMe().addMessage(name,msg);
	}
	template<typename INDEX,typename RVAL,typename T,typename T1,typename T2,typename T3,typename T4,typename T5>
	void bind(INDEX name, RVAL (T::*func)(T1 arg1,T2 arg2,T3 arg3,T4 arg4,T5 arg5))
	{
		omessage<RVAL,T,T1,T2,T3,T4,T5> *message = new omessage<RVAL,T,T1,T2,T3,T4,T5>();
		message->bind(func);
		message->set(name);
		Messages::getMe().addMessage(name,message);
	}

	template<typename INDEX,typename RVAL,typename T1,typename T2,typename T3,typename T4,typename T5>
	std::string build(INDEX func,T1 t1,T2 t2,T3 t3,T4 t4,T5 t5)
	{
		message<RVAL,typename charstostring<T1>::type,typename charstostring<T2>::type,typename charstostring<T3>::type,typename charstostring<T4>::type,typename charstostring<T5>::type> msg;
		msg.set(func);
		return msg.build(t1,t2,t3,t4,t5);
	}
}
