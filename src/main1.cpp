#include <iostream>
#include <memory>
#include <utility>
#include <typeindex>
#include <type_traits>
#include <map>
#include <vector>
#include <string>

// c++14 add "add_pointer_t" helper
// #if !defined(_MSC_VER) || ( _MSC_VER < 1800 )
// namespace std {
//     template <typename T>
// 	using add_pointer_t = typename std::add_pointer<T>::type;
// }
// #endif

template < typename Key, typename Object >
class Factory {
private:
	using FakeCreatorFunc = std::add_pointer_t< Object* () >;
	template < typename... Args >
	using RealCreatorFunc = std::add_pointer_t< Object* ( Args... ) >;

	using RealKey    = std::pair< Key, std::type_index >;
	using CreatorMap = std::map< RealKey, FakeCreatorFunc >;

	template < typename T, typename... Args >
	static Object * CreateFunc( Args... args ) {
		return new T { std::forward<Args>( args )... };
	}

    static CreatorMap& getMap() {
        static CreatorMap creators;
        return creators;
    }

public:
	using ResultType = std::unique_ptr<Object>;

	template < typename T, typename... Args >
	static typename std::enable_if< std::is_base_of<Object, T>::value &&
    std::is_constructible<T, Args...>::value, bool >::type RegisterItem( Key key ) {
        RealKey realKey { key, std::type_index( typeid( RealCreatorFunc<Args...> ) ) };
        auto it = getMap().find( realKey );
        if ( it != getMap().end( ) )
            return false;
        getMap()[ realKey ] = reinterpret_cast<FakeCreatorFunc>( CreateFunc<T, Args...> );
            std::cout << "11111111111111111111 key : " << key << " typeid: " << typeid( RealCreatorFunc<Args...>).name() << " size is " << sizeof...(Args) <<'\n';
            std::cout << "11111111111111111111 key : " << key << " typeid: " << typeid( CreateFunc<T, Args...>).name() <<'\n';
			return true;
		}

	template < typename... Args >
	static ResultType Create( Key key, Args... args ) {
		RealKey realKey { key, std::type_index( typeid( RealCreatorFunc<Args...> ) ) };
		auto it = getMap().find( realKey );

        std::cout << "22222222222 key: " << key << " typeid: " << typeid( RealCreatorFunc<Args...>).name() << " size is " << sizeof...(args) << '\n';
            // std::cout << std::forward<Key>(key);
            // using expander = int[];
            // (void)expander{0, (void(std::cout << ',' << std::forward<Args>(args)),0)...};
		if ( it == getMap().end( ) )
			return {};
		auto creator = reinterpret_cast<RealCreatorFunc<Args...>>( it->second );
		return std::unique_ptr<Object> { creator( std::forward<Args>( args )... ) };
	}
};

struct Shape {
	virtual ~Shape( ) {}
	virtual void Display( std::ostream& ) const = 0;
protected:
	Shape( ) {}
};

struct Circle : public Shape {
	Circle( ) = default;
	Circle( float rad ) : radius_ { rad } {}
	void Display( std::ostream& os ) const override { os << "Circle ( " << radius_ << " )"; }
private:
	float radius_ = 1.f;
};

struct Rect : public Shape {
	Rect( ) = default;
	Rect( float w, float h ) : w_ { w }, h_ { h } {}
	void Display( std::ostream& os) const override { os << "Rect ( " << w_ << " x " << h_ << " )"; }
private:
	float w_ = 5.f;
	float h_ = 5.f;
};

struct Config {
    float a;
    std::string b;
};

struct Rr : public Shape {
    Rr() = default;
    Rr(Config config) : mConfig{ config } {}
    void Display(std::ostream& os) const override {
        os << "Rr a:" << mConfig.a << " b:" << mConfig.b;
    }
private:
    Config mConfig;
};

struct SRr : public Shape {
	SRr( ) = default;
	SRr(std::string const& name, int i ) : name_{ name }, i_{ i } {}
	void Display( std::ostream& os ) const override { os << "Display => SRr ( " << name_ <<  " i is " << i_ << " )"; }
private:
    std::string name_ = "haha";
    int i_;
};


enum class Shapes {
	CIRCLE = 0,
	RECT,
};


std::ostream& operator<<( std::ostream& os, Config config) {

	os << "config a is " << config.a << " b is " << config.b;
	// if ( ptr ) {
	// 	os << "config a is " << ptr->a << " b is " << ptr->b;
	// } else {
	// 	os << "Empty Config";
	// }
	return os;
}


std::ostream& operator<<( std::ostream& os, std::unique_ptr<Shape> const & ptr ) {
	if ( ptr ) {
		ptr->Display( os );
	} else {
		os << "Empty Shape";
	}
	return os;
}

#define FACTORY_REGISTER(base, child) \
    class child;\
    extern SIMPLE_API base::DerivedRegister<child> _reg ##child (#child);

int main( ) {
	// Factory< Shapes, Shape > Factory< Shapes, Shape >;
    Config config = {.5, "sss"};

	// Factory< Shapes, Shape >::RegisterItem<Circle, float>( Shapes::CIRCLE );
	// Factory< Shapes, Shape >::RegisterItem<Rect, float, float>( Shapes::RECT );
	// Factory< Shapes, Shape >::RegisterItem<Rect>( Shapes::RECT );
	// Factory< Shapes, Shape >::RegisterItem<Rr, Config>( Shapes::RECT );

    std::string s = "ss";
	Factory< std::string, Shape >::RegisterItem<Rr, Config>( s );
	Factory< std::string, Shape >::RegisterItem<SRr, std::string, int >( s );

	std::vector< std::unique_ptr<Shape> > shapes;
	shapes.reserve( 16 );
	
	// shapes.push_back( std::move( Factory< Shapes, Shape >::Create( Shapes::CIRCLE, 2.f ) ) );
	// shapes.push_back( std::move( Factory< Shapes, Shape >::Create( Shapes::CIRCLE, 1 ) ) );
	// shapes.push_back( std::move( Factory< Shapes, Shape >::Create( Shapes::RECT, 1 ) ) );
	// shapes.push_back( std::move( Factory< Shapes, Shape >::Create( Shapes::RECT, 3.f, 4.f ) ) );
	// shapes.push_back( std::move( Factory< Shapes, Shape >::Create( Shapes::RECT ) ) );
	// shapes.push_back( std::move( Factory< Shapes, Shape >::Create( Shapes::RECT, config ) ) );
	shapes.push_back( std::move( Factory< std::string, Shape >::Create( "ssss", config ) ) );
	shapes.push_back( std::move( Factory< std::string, Shape >::Create( "ss", std::string("asdfaq"), 18 ) ) );
	
	std::cout.precision( 2 ); std::cout << std::fixed;

	for( auto const & shape : shapes ) 
		std::cout << shape << std::endl;

	return 0;
}
