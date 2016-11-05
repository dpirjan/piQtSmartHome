#include "io.h"

io::io()
    : m_system()
    , m_hardware()
    , m_type()
    , m_category()
    , m_zone()
    , m_node()
    , m_address()
{
}

io::io(const QString &system,
       const QString &hardware,
       const QString &type,
       const QString &category,
       const QString &zone,
       const QString &node,
       const QString &address)
    : m_system(system)
    , m_hardware(hardware)
    , m_type(type)
    , m_category(category)
    , m_zone(zone)
    , m_node(node)
    , m_address(address)
{
}

io::io(const io &obj)
{
    m_system = obj.m_system;
    m_hardware = obj.m_hardware;
    m_type = obj.m_type;
    m_category = obj.m_category;
    m_zone = obj.m_zone;
    m_node = obj.m_node;
    m_address = obj.m_address;
}

io& io::operator =(const io &other)
{
    m_system = other.m_system;
    m_hardware = other.m_hardware;
    m_type = other.m_type;
    m_category = other.m_category;
    m_zone = other.m_zone;
    m_node = other.m_node;
    m_address = other.m_address;

    return *this;
}

io::~io()
{
}

QString io::getSystemType() const
{
    return m_system;
}

QString io::getHardwareType() const
{
    return m_hardware;
}

QString io::getType() const
{
    return m_type;
}

QString io::getCategory() const
{
    return m_category;
}

QString io::getZone() const
{
    return m_zone;
}

QString io::getNode() const
{
    return m_node;
}

QString io::getAddress() const
{
    return m_address;
}

void io::setSystemType(const QString &system)
{
    m_system = system;
}

void io::setHardwareType(const QString &hardware)
{
    m_hardware = hardware;
}

void io::setType(const QString &type)
{
    m_type = type;
}

void io::setCategory(const QString &category)
{
    m_category = category;
}

void io::setZone(const QString &zone)
{
    m_zone = zone;
}

void io::setNode(const QString &node)
{
    m_node = node;
}

void io::setAddress(const QString &address)
{
    m_address = address;
}

void io::registerMetaType()
{
    qRegisterMetaType<io>("io");
    qDBusRegisterMetaType<io>();

    qRegisterMetaType<QList<io> >("QList<io>");
    qDBusRegisterMetaType<QList<io> >();
}

QDBusArgument &operator<<(QDBusArgument &argument, const io &data)
{
    argument.beginStructure();
    QString system = data.getSystemType();
    QString hardware = data.getHardwareType();
    QString type = data.getType();
    QString category = data.getCategory();
    QString zone = data.getZone();
    QString node = data.getNode();
    QString address = data.getAddress();
    argument << system;
    argument << hardware;
    argument << type;
    argument << category;
    argument << zone;
    argument << node;
    argument << address;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, io &data)
{
    QString system, hardware, type, category, zone, node, address;
    argument.beginStructure();
    argument >> system;
    argument >> hardware;
    argument >> type;
    argument >> category;
    argument >> zone;
    argument >> node;
    argument >> address;
    argument.endStructure();
    data.setSystemType(system);
    data.setHardwareType(hardware);
    data.setType(type);
    data.setCategory(category);
    data.setNode(node);
    data.setZone(zone);
    data.setAddress(address);
    return argument;
}
