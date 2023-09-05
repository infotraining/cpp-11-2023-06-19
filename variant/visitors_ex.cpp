#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace std;

/////////////////////////////////////
// IsSimilar

template <typename T1, typename T2>
struct IsSimilar
{
    static constexpr bool value = std::is_same_v<std::decay_t<T1>, std::decay_t<T2>>;
};

template <typename T1, typename T2>
constexpr bool IsSimilar_v = IsSimilar<T1, T2>::value;

static_assert(IsSimilar_v<int, const int>);
static_assert(IsSimilar_v<int, const int&>);
static_assert(IsSimilar_v<int, int&>);

namespace ClassicPoly
{
    struct Shape
    {
        virtual ~Shape() = default;
        virtual double area() const = 0;
    };

    struct Circle : Shape
    {
        int radius;

        Circle(int r) : radius(r)
        {}

        double area() const override
        {
            return radius * radius * 3.1415;
        }
    };

    struct Rectangle : Shape
    {
        int width, height;

        Rectangle(int w, int h) : width{w}, height{h}
        {}

        double area() const override
        {
            return width * height;
        }
    };

    struct Square : Shape
    {
        int size;

        Square(int s) : size{s}
        {}

        double area() const override
        {
            return size * size;
        }
    };
}

namespace VariantPoly
{
    struct Circle
    {
        int radius;   

        double area() const
        {
            return radius * radius * 3.1415;
        }
    };

    struct Rectangle
    {
        int width, height;    

        double area() const
        {
            return width * height;
        }
    };

    struct Square
    {
        int size; 

        double area() const
        {
            return size * size;
        }
    };

    //template<typename T1, typename T2>
    //struct is_similar
    //{
    //    static constexpr bool value = 
    //};

    // Polymorphic wrapper
    struct Shape
    {
        using ShapeType = variant<Circle, Rectangle, Square>;

        ShapeType shp;

        template <typename T> 
        Shape(T&& s) : shp{std::forward<T>(s)}
        {}        

        double area() const
        {
            return std::visit([](const auto& s) { return s.area(); }, shp);
        }
    };
}

//////////////////////
// overload

template <typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...; // C++17
};

// required in C++17 only
template <typename... Ts>
overload(Ts...) -> overload<Ts...>;

TEST_CASE("visit a shape variant and calculate area")
{
    using namespace VariantPoly;

    using Shape = variant<Circle, Rectangle, Square>;

    vector<Shape> shapes = {Circle{1}, Square{10}, Rectangle{10, 1}};

    double total_area{};

    overload shape_visitor{
        [&total_area](const Circle& c) { total_area += c.radius * c.radius * 3.1415; },
        [&total_area](const Rectangle& r) { total_area += r.width * r.height; },
        [&total_area](const Square& s) { total_area += s.size * s.size; }
    };

    for(const auto& shp : shapes)
    {
        std::visit(shape_visitor, shp);
    }

    REQUIRE_THAT(total_area, Catch::Matchers::WithinAbs(113.14, 0.01));

    total_area = 0.0;

    overload see = overload{
        [](const Circle& arg) -> double { return arg.radius * arg.radius * 3.14; },
        [](const Square& arg) -> double { return arg.size * arg.size; },
        [](const Rectangle& arg) -> double { return arg.width * arg.height; }
    };

    for( const auto& element : shapes ) 
        total_area += visit(see, element);

    //REQUIRE(total_area == Approx(113.14));
    REQUIRE_THAT(total_area, Catch::Matchers::WithinAbs(113.14, 0.01));
}

TEST_CASE("classic poly")
{
    using namespace ClassicPoly;
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Circle>(1)); 
    shapes.push_back(std::make_unique<Rectangle>(10, 1));
    shapes.push_back(std::make_unique<Square>(10));

    double total_area = 0.0;
    for(const auto& shp : shapes)
    {
        total_area += shp->area();
    }

    std::cout << "Total area: " << total_area << "\n";
}

TEST_CASE("variant poly")
{
    using namespace VariantPoly;
    std::vector<Shape> shapes;

    shapes.push_back(Circle{1});
    shapes.push_back(Rectangle{10, 1});
    shapes.push_back(Square{10});

    double total_area = 0.0;
    for (const auto& shp : shapes)
    {
        total_area += shp.area();
    }

    std::cout << "Total area: " << total_area << "\n";

    Shape shp1 = Circle{2};
    //Shape shp2 = shp1; // cc
    //CHECK(shp1.area() == shp2.area());
}

int foo(int arg)
{
    return arg;
}

template <typename T>
struct SizeOf
{
    static constexpr int value = sizeof(T);
};

template <typename T>
constexpr int SizeOf_v = SizeOf<T>::value;

template <typename T>
struct Identity
{
    using type = T;    
};

template <typename T>
using Identity_t = typename Identity<T>::type;

TEST_CASE("meta-functions")
{
    CHECK(foo(42) == 42);

    static_assert(SizeOf<int>::value == 4);
    static_assert(SizeOf_v<int> == 4);

    static_assert(std::is_same_v<Identity<int>::type, int>);
    static_assert(std::is_same_v<Identity_t<int>, int>);
}

//////////////////////////////
// traits


///////////////////////////////
// IsVoid

template <typename T>
struct IsVoid
{
    static constexpr bool value = false;
};

template <>
struct IsVoid<void>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool IsVoid_v = IsVoid<T>::value;


///////////////////////////////////
// IsPointer

template <typename T>
struct IsPointer
{
    static constexpr bool value = false;
};

template <typename T>
struct IsPointer<T*>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool IsPointer_v = IsPointer<T>::value;

using T1 = int;
using T2 = void;
using T3 = double*;

static_assert(std::is_void_v<T1> == false);
static_assert(IsVoid_v<T2> == true);

static_assert(IsPointer_v<T3>);

template <typename T>
void use_pointer(T* ptr)
{
    static_assert(!std::is_void_v<T>);

    using NewType = std::remove_const_t<T>;

    if (ptr)
    {
        std::remove_const_t<T> temp = *ptr;

        ++temp;
    }
}

template <typename T>
struct RemoveConst
{
    using type = T;
};

template <typename T>
struct RemoveConst<const T>
{
    using type = T;
};

template <typename T>
using RemoveConst_t = typename RemoveConst<T>::type;

TEST_CASE("using traits")
{    
    void* raw_mem{};
    //use_pointer(raw_mem);

    const int x = 10;
    use_pointer(&x);

    using MyConstType = const int;

    static_assert(std::is_same_v<RemoveConst_t<MyConstType>, int>);    
}



