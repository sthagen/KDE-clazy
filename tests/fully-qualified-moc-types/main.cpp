#include <QtCore/QObject>
#include <QtDBus/QDBusPendingReply>
#include <memory>
struct A {};
struct NonNamespacedGadget {
    Q_GADGET
};

namespace NS {
    struct MyType {};

    struct NamespacedGadget {
        Q_GADGET
    };

    enum EnumFoo { EnumFoo1 };

    class MyObject : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(NS::MyType foo READ foo) // OK, not gadget
        Q_PROPERTY(MyType foo1 READ foo) // OK, not gadget
        Q_PROPERTY(EnumFoo enumFoo READ enumFoo CONSTANT) // OK
        Q_PROPERTY(NamespacedGadget namespacedGadget READ namespacedGadget CONSTANT) // Warn, gadget
        Q_PROPERTY(NS::NamespacedGadget namespacedGadget2 READ namespacedGadget2 CONSTANT) // OK
        Q_PROPERTY(NonNamespacedGadget nonNamespacedGadget READ nonNamespacedGadget CONSTANT) // OK
    Q_SIGNALS:
        void mysig(NS::MyType);
        void mysig2(MyType &); // Warn
        void mysig3(NS::MyType);
        void mysig4(const NS::MyType &);
        void mysig5(A);
        void mysig6(const A);
        void mysig7(const A *);
        void mysig8(A *);
    public Q_SLOTS:
        void myslot1(NS::MyType);
        void myslot2(MyType); // Warn
        void myslot3(NS::MyType);
        void myslot4(const NS::MyType &);
        void myslot5(A);
        void myslot6(const A);
        void myslot7(const A *);
        void myslot8(A *);
    public:
        Q_INVOKABLE void myinvokable1(NS::MyType);
        Q_INVOKABLE void myinvokable2(MyType); // Warn
        Q_INVOKABLE void myinvokable3(NS::MyType);
        Q_INVOKABLE void myinvokable4(const NS::MyType &);
        Q_INVOKABLE void myinvokable5(A);
        Q_INVOKABLE void myinvokable6(const A);
        Q_INVOKABLE void myinvokable7(const A *);
        Q_INVOKABLE void myinvokable8(A *);
        Q_INVOKABLE MyType* myinvokable9(NS::MyType); // Warn
        NS::MyType foo();
        NamespacedGadget namespacedGadget() const;
        NamespacedGadget namespacedGadget2() const;
        NonNamespacedGadget nonNamespacedGadget() const;
        EnumFoo enumFoo() const;
    };
}


template<typename T> using DummyListAlias = QList<T>;
namespace { // annonymous
    struct AnnonFoo {};
};

using namespace std; // pair<bool,QualMe> is returned for one method, the check should warn about the missing "std::" prefix
class MyObj2 : public QObject
{
public:
    struct QualMe {};
    using MyList = QList<QualMe>; // QualMe is not fully qualified here, but it shouldn't matter when using the typedef
Q_OBJECT
Q_SIGNALS:
    void mySig(AnnonFoo);
public Q_SLOTS:
    inline std::pair<bool,QualMe> unqualPairParam() {return {};} // Warn
    inline pair<bool,QualMe> unqualPairClass() {return {};} // Warn
    inline std::pair<bool, MyObj2::QualMe> fullyQUalPair() {return {};} // OK
    inline MyList typeAlias() {return {};} // WARN
    inline QList<QualMe> genericWithoutFullyQual() {return {};} // WARN
    inline QList<MyObj2::QualMe> genericFullyQual() {return {};} // OK
    inline QStringList qstringListTypealias() {return {};} // OK
    inline MyObj2::MyList fullTypeAlias() {return {};} // OK
    inline QDBusPendingReply<QualMe> unqualGenericDbusReply() {return {};} // WARN
    inline QDBusPendingReply<bool> boolDbusReply() {return {};} // OK
    inline QDBusPendingReply<> voidDbusReply() {return {};} // OK
    inline QDBusPendingReply<MyList> typedefInGeneric() {return {};} // WARN
    inline void nestedGeneric(QDBusPendingReply<std::shared_ptr<MyObj2>>) {} // OK
    inline void nestedNotFullyQualifiedGeneric(QDBusPendingReply<std::shared_ptr<MyList>>) {} // WARN
    inline const MyList& notQualWithModifier() {return lst;};
    DummyListAlias<int> myList() { return {1,2,3};};

    // const modifiers
    inline std::shared_ptr<const QualMe> returnSharedPtrNotQual() // WARN
    {
        return std::shared_ptr<const QualMe>(new QualMe());
    }
    inline std::shared_ptr<const MyObj2::QualMe> returnSharedPtr()
    {
        return std::shared_ptr<const QualMe>(new QualMe());
    }
    inline std::shared_ptr<const     MyObj2::QualMe> returnSharedPtrSpaces()
    {
        return std::shared_ptr<const QualMe>(new QualMe());
    }
    inline const MyObj2::QualMe returnConstNoGeneric()
    {
        return QualMe();
    }
private:
    MyList lst;
};

Q_DECLARE_METATYPE(MyObj2::QualMe);
Q_DECLARE_METATYPE(std::shared_ptr<MyObj2::MyList>);

#if QT_VERSION_MAJOR == 5
#include "main.qt5.moc_"
#elif Q_MOC_OUTPUT_REVISION == 69
#include "main.qt6.moc_69"
#else
#include "main.qt6.moc_"
#endif
