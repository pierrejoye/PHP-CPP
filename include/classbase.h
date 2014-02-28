/**
 *  ClassBase.h
 *
 *  This is the base class of the "Class" class. This is an internal class that
 *  is used by the PHP-CPP library. But because the constructor is protected,
 *  you can not create any instances if this class yourself (and you are not
 *  supposed to do that either.
 *
 *  Further more, because this base class is a 'private' base of Class, all 
 *  features of it are normally also inaccessible.
 *
 *  In other words: it is not meant to be directly used by extension writers.
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace Php {

/**
 *  Method signatures
 */
typedef void    (Base::*method_callback_0)();
typedef void    (Base::*method_callback_1)(Parameters &);
typedef Value   (Base::*method_callback_2)();
typedef Value   (Base::*method_callback_3)(Parameters &);

/**
 *  Forward declarations
 */
class Method;

/**
 *  Class definition
 */
class ClassBase
{
protected:
    /**
     *  Protected constructor
     *  @param  classname   Class name
     *  @param  flags       The class flags
     */
    ClassBase(const char *classname, int flags = 0) : _name(classname), _flags(flags) {}
    
public:
    /**
     *  Copy constructor
     *  @param  that
     * 
     *  @todo   add properties
     *  @todo   prefer move
     */
    ClassBase(const ClassBase &that) : 
        _name(that._name), _flags(that._flags), _methods(that._methods) {}

    /**
     *  Move constructor
     *  @param  that
     * 
     *  @todo   add properties
     *  @todo   use move semantics
     */
    ClassBase(ClassBase &&that) :
        _flags(that._flags), _methods(std::move(that._methods)), _entry(that._entry) 
    {
        // other entry are invalid now (not that it is used..., class objects are
        // only moved during extension setup, when the entry pointer has not yet
        // been allocated)
        that._entry = nullptr;
    }

    /**
     *  Destructor
     */
    virtual ~ClassBase();
    
    /**
     *  Construct a new instance of the object
     *  @return Base
     */
    virtual Base* construct() = 0;

    /**
     *  Initialize the class, given its name
     * 
     *  The module functions are registered on module startup, but classes are
     *  initialized afterwards. The Zend engine is a strange thing. Nevertheless,
     *  this means that this method is called after the module is already available.
     *  This function will inform the Zend engine about the existence of the
     *  class.
     */
    void initialize();
    
protected:
    /**
     *  Add a method to the class
     *  
     *  The method will be accessible as one of the class methods in your PHP
     *  code. When the method is called, it will automatically be forwarded
     *  to the C++ implementation. The flags can be Php::Public, Php::Protected
     *  or Php::Private (using private methods can be useful if you for example
     *  want to make the __construct() function private). The access-modified
     *  flag can be bitwise combined with the flag Php::Final or Php::Abstract).
     * 
     *  @param  name        Name of the method
     *  @param  method      The actual method
     *  @param  flags       Optional flags
     *  @param  args        Description of the supported arguments
     */
    void add(const char *name, method_callback_0, int flags=0, const Arguments &args = {});
    void add(const char *name, method_callback_1, int flags=0, const Arguments &args = {});
    void add(const char *name, method_callback_2, int flags=0, const Arguments &args = {});
    void add(const char *name, method_callback_3, int flags=0, const Arguments &args = {});

private:
    /**
     *  Retrieve an array of zend_function_entry objects that hold the 
     *  properties for each method. This method is called at extension
     *  startup time to register all methods.
     * 
     *  @param  classname       The class name
     *  @return zend_function_entry[]
     */
    const struct _zend_function_entry *entries();

    /**
     *  Name of the class
     *  @var    string
     */
    std::string _name;

    /**
     *  The class flags (this can be values like Php::Abstract and Php::Final)
     *  @var    int
     */
    int _flags = 0;

    /** 
     *  The class entry
     *  @var    zend_class_entry
     */
    struct _zend_class_entry *_entry = nullptr;

    /**
     *  Pointer to the entries
     *  @var    zend_function_entry[]
     */
    struct _zend_function_entry *_entries = nullptr;
    
    /**
     *  All class methods, this is a map indexed by method name
     *  @var    set
     */
    std::set<std::shared_ptr<Method>> _methods;
    
    /**
     *  All class properties, also a map indexed by name
     *  @var    Properties
     */
//    std::map<std::string,Property> _properties;
};
    
/**
 *  End namespace
 */
}
