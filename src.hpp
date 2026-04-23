
#ifndef SRC_HPP
#define SRC_HPP

#include <stdexcept>
#include <initializer_list>
#include <typeinfo>
#include <vector>
#include <map>
#include <set>
#include <utility>

namespace sjtu {

class any_ptr {
private:
    // Base control block for reference counting
    struct control_block {
        int ref_count;
        
        control_block() : ref_count(1) {}
        virtual ~control_block() = default;
        virtual const std::type_info& type() const = 0;
        virtual void* get_ptr() const = 0;
        virtual const void* get_const_ptr() const = 0;
    };
    
    // Template control block that holds actual pointer and type info
    template<typename T>
    struct control_block_impl : control_block {
        T* ptr;
        
        control_block_impl(T* p) : ptr(p) {}
        
        virtual ~control_block_impl() {
            delete ptr;
        }
        
        virtual const std::type_info& type() const override {
            return typeid(T);
        }
        
        virtual void* get_ptr() const override {
            return static_cast<void*>(ptr);
        }
        
        virtual const void* get_const_ptr() const override {
            return static_cast<const void*>(ptr);
        }
    };
    
    control_block* ctrl;
    
    // Helper function to decrease reference count and clean up if needed
    void dec_ref() {
        if (ctrl) {
            --ctrl->ref_count;
            if (ctrl->ref_count == 0) {
                delete ctrl;
                ctrl = nullptr;
            }
        }
    }
    
    // Helper function to increase reference count
    void inc_ref() {
        if (ctrl) {
            ++ctrl->ref_count;
        }
    }

public:
    /**
     * @brief 默认构造函数，行为应与创建空指针类似
     * 
     */
    any_ptr() : ctrl(nullptr) {}

    /**
     * @brief 拷贝构造函数，要求拷贝的层次为浅拷贝，即该对象与被拷贝对象的内容指向同一块内存
     * @note 若将指针作为参数传入，则指针的生命周期由该对象管理
     * @example
     *  any_ptr a = make_any_ptr(1);
     *  any_ptr b = a;
     *  a.unwrap<int> = 2;
     *  std::cout << b << std::endl; // 2
     * @param other
     */
    any_ptr(const any_ptr &other) : ctrl(other.ctrl) {
        inc_ref();
    }
    
    template <class T> any_ptr(T *ptr) : ctrl(new control_block_impl<T>(ptr)) {}

    /**
     * @brief 析构函数，注意若一块内存被多个对象共享，那么只有最后一个析构的对象需要释放内存
     * @example
     *  any_ptr a = make_any_ptr(1);
     *  {
     *    any_ptr b = a;
     *  }
     *  std::cout << a << std::endl; // valid
     * @example
     *  int *p = new int(1);
     *  {
     *    any_ptr a = p;
     *  }
     *  std::cout << *p << std::endl; // invalid
     * 
     */
    ~any_ptr() {
        dec_ref();
    }

    /**
     * @brief 拷贝赋值运算符，要求拷贝的层次为浅拷贝，即该对象与被拷贝对象的内容指向同一块内存
     * @note 应与指针拷贝赋值运算符的语义相近
     * @param other
     * @return any_ptr&
     */
    any_ptr &operator=(const any_ptr &other) {
        if (this != &other) {
            dec_ref();
            ctrl = other.ctrl;
            inc_ref();
        }
        return *this;
    }
    
    template <class T> any_ptr &operator=(T *ptr) {
        dec_ref();
        ctrl = new control_block_impl<T>(ptr);
        return *this;
    }

    /**
     * @brief 获取该对象指向的值的引用
     * @note 若该对象指向的值不是 T 类型，则抛出异常 std::bad_cast
     * @example
     *  any_ptr a = make_any_ptr(1);
     *  std::cout << a.unwrap<int>() << std::endl; // 1
     * @tparam T
     * @return T&
     */
    template <class T> T &unwrap() {
        if (!ctrl) {
            throw std::bad_cast();
        }
        if (ctrl->type() != typeid(T)) {
            throw std::bad_cast();
        }
        return *static_cast<T*>(ctrl->get_ptr());
    }
    
    // const version of unwrap
    template <class T> const T &unwrap() const {
        if (!ctrl) {
            throw std::bad_cast();
        }
        if (ctrl->type() != typeid(T)) {
            throw std::bad_cast();
        }
        return *static_cast<const T*>(ctrl->get_const_ptr());
    }
};

/**
 * @brief 由指定类型的值构造一个 any_ptr 对象
 * @example
 *  any_ptr a = make_any_ptr(1);
 *  any_ptr v = make_any_ptr<std::vector<int>>(1, 2, 3);
 *  any_ptr m = make_any_ptr<std::map<int, int>>({{1, 2}, {3, 4}});
 * @tparam T
 * @param t
 * @return any_ptr
 */
template <class T> any_ptr make_any_ptr(const T &t) { return any_ptr(new T(t)); }

// Specialized versions for common container types
template <class T> any_ptr make_any_ptr(std::initializer_list<T> init) {
    return any_ptr(new std::vector<T>(init));
}

template <class K, class V> any_ptr make_any_ptr(std::initializer_list<std::pair<const K, V>> init) {
    return any_ptr(new std::map<K, V>(init));
}

template <class T> any_ptr make_any_ptr(std::initializer_list<T> init, typename std::enable_if<std::is_same<T, typename std::set<T>::value_type>::value>::type* = 0) {
    return any_ptr(new std::set<T>(init));
}

}  // namespace sjtu

#endif
