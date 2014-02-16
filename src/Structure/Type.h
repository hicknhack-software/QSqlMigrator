#ifndef STRUCTURE_SQLTYPE_H
#define STRUCTURE_SQLTYPE_H

#include "config.h"

#include <QString>

namespace Structure {

/*!
 * \brief value object that represent any sql type
 */
class QSQLMIGRATOR_DLL_EXPORT Type
{
public:
    /*!
     * \brief Base Type enum
     */
    enum Base {
        Invalid,
        Char, //!< precision is length (defaults to 1)
        VarChar, //!< precision is maximum length (defaults to 255)
        String = VarChar,
        Binary, //!< precision represents length in bytes (required)
        VarBinary, //!< precision represents maximum bytes
        ByteArray = VarBinary, //!< Alias vor VarBinary
        Boolean,
        SmallInt, //!< precision is 5
        Integer, //!< precision defaults to 10
        BigInt, //!< precision is 19
        Decimal, //!< precision and scale required (total digits, digits after comma)
        Numeric = Decimal, //!< Alias for Decimal
        Float,
        DoublePrecision,
        Double = DoublePrecision, //!< Alias for Double Precision
        Date,
        Time,
        Timestamp,
        DateTime = Timestamp, //!< Alias for Timestamp
        Interval,
        Array,
        Multiset,
        Xml,
        Custom = 100, //!< Custom Types start here (the database adapter has to be able to convert this)
    };

    /*!
     * \brief Construct the type from
     * \param sqlType the string representation of an sql type
     */
    Type(const QString& string);

    /*!
     * \brief Constructs the type
     * \param base the required base type
     * \param precision if supported or required the precision or length
     * \param scale if supported or required by the base type
     */
    Type(Base base, quint64 precision = 0, int scale = 0);

    //! \return true, if the string based type is used
    bool isString() const;

    //! \return true, if the base type is used
    bool isBaseType() const;

    //! \return the string representation of the sql type (if string form is used)
    const QString& string() const;

    //! \return the base type (if base form is used)
    Base base() const;

    //! \return the precision (only used for supported types - numeric and string)
    quint64 precision(quint64 defaultValue = 0) const;

    //! \return the scale of the type (only used for numeric values)
    int scale() const;

    //! \return the invalid SqlType
    static const Type& invalid();

    //! \return true if this type is not invalid
    bool isValid() const;

private:
    Base m_base;
    quint64 m_precision;
    int m_scale;
    QString m_string;
};

} // namespace Structure

#endif // STRUCTURE_SQLTYPE_H
