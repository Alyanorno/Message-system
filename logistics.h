#pragma once
#include <vector>
#include <string>
#include <typeinfo>
#include <map>

class Logistic
{
public:
	Logistic();
	~Logistic();
	void Initialize();

	struct Object
	{
		struct Id
		{
			Id( int _owner, int _id ) : owner(_owner), id(_id) {}
			int owner, id;
		};
		std::vector< Id > ids;
	};
	std::vector< Object > objects;
	
	struct Message
	{
		template< typename T >
		Message( std::string _to, std::string _info, T& _message ) : to(_to), info(_info), received(false)
			{ message = new T(_message); type = typeid(T).name(); }
		Message( const Message& m ) : to(m.to), info(m.info), type(m.type), received(m.received)
			{ message = m.message; }
		~Message() {}
		template< typename T >
		T ReceiveMessage() 
		{
			if( type != typeid(T).name() )
				throw std::string( "Incorrect type" );
			else if( received )
				throw std::string( "Message already received" );
			received = true;
			T* ptr = reinterpret_cast<T*>(message);
			T t( *ptr );
			delete ptr;
			return t;
		}
		template< typename T >
		bool IsMessageType() { return type == typeid(T).name(); }
		std::string to;
		std::string info;
	private:
		std::string type;
		bool received;
		void* message;
	};
	int MessageCount( std::string to ) 
	{
		if( recipients.count( to ) == 0 )
			return 0;
		else
			return messages[ recipients[ to ] ].size();
	}
	Message GetMessage( std::string to )
	{
		if( recipients.count( to ) == 0 || messages[ recipients[ to ] ].size() == 0 )
			throw std::string( "No messages to: " + to );
		Message result( messages[ recipients[ to ] ].back() );
		messages[ recipients[ to ] ].pop_back();
		return result;
	}
	// Some very odd error where the compiler adds an A to GetMessage when you call GetMessage
	// Temporary fix until i find out what the hell is going on
	Message GetMessageA( std::string to )
	{
		return GetMessage( to );
	}
	void AddMessage( Message message )
	{
		if( recipients.count( message.to ) == 0 ) {
			messages.push_back( std::vector< Message >() );
			recipients.insert( std::pair<std::string,int>( message.to, messages.size() - 1 ) );
		}
		messages[ recipients[ message.to ] ].push_back( message );
	}

private:
	std::map< std::string, int > recipients;
	std::vector< std::vector< Message > > messages;
};

