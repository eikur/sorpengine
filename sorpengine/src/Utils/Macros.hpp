#pragma once

// Defines a unique ptr of a class member, and its getter
#define MEMBER_DECL( _CLASS_, _NAME_, _GETTER_ )	\
		private:								\
			std::unique_ptr<_CLASS_> _NAME_;	\
		public:									\
			_CLASS_& _GETTER_() const	\
			{							\
				return *_NAME_.get();	\
			}

// Defines a member, its const getter and its getter
#define MEMBER_CONST_GET_SET( _CLASS_, _NAME_, _GETTER_, _SETTER_ )	\
		private:								\
			_CLASS_ _NAME_;	\
		public:									\
			const _CLASS_& _GETTER_() const	\
			{							\
				return _NAME_;	\
			} \
			void _SETTER_(int value) \
			{ \
				_NAME_ = value; \
			}

// Defines a member, its const getter 
#define MEMBER_CONST_GET( _CLASS_, _NAME_, _GETTER_)	\
		private:								\
			_CLASS_ _NAME_;	\
		public:									\
			const _CLASS_& _GETTER_() const	\
			{							\
				return _NAME_;	\
			} 

// Defines a member, its const getter 
#define MEMBER_GET( _CLASS_, _NAME_, _GETTER_)	\
		private:								\
			_CLASS_ _NAME_;	\
		public:									\
			_CLASS_& _GETTER_()	\
			{							\
				return _NAME_;	\
			} 
