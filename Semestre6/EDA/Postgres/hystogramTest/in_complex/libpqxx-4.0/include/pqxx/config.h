/* include/pqxx/config.h.  Generated from config.h.in by configure.  */
/* include/pqxx/config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `pq' library (-lpq). */
#define HAVE_LIBPQ 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "libpqxx"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "Jeroen T. Vermeulen <jtv@xs4all.nl>"

/* Define to the full name of this package. */
#define PACKAGE_NAME "libpqxx"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libpqxx 4.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libpqxx"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "4.0"

/* name of standard library namespace (normally "std") */
#define PGSTD std

/* namespace of TR1 standard extensions (e.g. "std" or "std::tr1") */
#define PQXXTR1 std::tr1

/* Define if <iterator> lacks an iterator template definition */
/* #undef PQXX_BROKEN_ITERATOR */

/* Define if function overloading using "using" breaks */
/* #undef PQXX_BROKEN_USING_DECL */

/* Define if compiler has auto_ptr */
#define PQXX_HAVE_AUTO_PTR 1

/* Define if you have the <boost/smart_ptr.hpp> header */
/* #undef PQXX_HAVE_BOOST_SMART_PTR */

/* Define if std provides the char_traits template and <char> specialization
   */
#define PQXX_HAVE_CHAR_TRAITS 1

/* Define if count_if() works according to the standard */
#define PQXX_HAVE_COUNT_IF 1

/* Define if preprocessor supports pragma "message" */
#define PQXX_HAVE_CPP_PRAGMA_MESSAGE 1

/* Define if preprocessor supports nonstandard "warning" directive */
#define PQXX_HAVE_CPP_WARNING 1

/* Define if math.h defines C99 isinf() */
/* #undef PQXX_HAVE_C_ISINF */

/* Define if math.h defines C99 isnan() */
/* #undef PQXX_HAVE_C_ISNAN */

/* Define if C math header defines NAN */
#define PQXX_HAVE_C_NAN 1

/* Define if distance() works according to the standard */
#define PQXX_HAVE_DISTANCE 1

/* Define if g++ supports const attribute */
#define PQXX_HAVE_GCC_CONST 1

/* Define if g++ supports deprecated attribute, as in g++ 4.0 */
#define PQXX_HAVE_GCC_DEPRECATED 1

/* Define if g++ supports noreturn attribute */
#define PQXX_HAVE_GCC_NORETURN 1

/* Define if g++ supports pure attribute */
#define PQXX_HAVE_GCC_PURE 1

/* Define if g++ supports visibility attribute, as in g++ 4.0 */
#define PQXX_HAVE_GCC_VISIBILITY 1

/* Define if stringstream::imbue works */
#define PQXX_HAVE_IMBUE 1

/* Define if you have the <ios> header */
#define PQXX_HAVE_IOS 1

/* Define if <limits> exists */
#define PQXX_HAVE_LIMITS 1

/* Define if you have the <locale> header */
#define PQXX_HAVE_LOCALE 1

/* Define if the compiler supports the standard "long double" type */
#define PQXX_HAVE_LONG_DOUBLE 1

/* Define if the compiler supports "long long" types */
#define PQXX_HAVE_LONG_LONG 1

/* Define if libpq has lo_tell() */
#define PQXX_HAVE_LO_TELL 1

/* Define if compiler has move(). */
/* #undef PQXX_HAVE_MOVE */

/* Define if C99 fnan()/nan()/lnan() are available */
#define PQXX_HAVE_NAN 1

/* Define if the system has the poll() function (mainly GNU/Linux) */
#define PQXX_HAVE_POLL 1

/* Define if libpq has PQcancel() and friends */
#define PQXX_HAVE_PQCANCEL 1

/* Define if libpq has PQclientEncoding() */
#define PQXX_HAVE_PQCLIENTENCODING 1

/* Define if libpq has PQdescribePortal() */
#define PQXX_HAVE_PQDESCRIBEPORTAL 1

/* Define if libpq has PQencryptPassword() */
#define PQXX_HAVE_PQENCRYPTPASSWORD 1

/* Define if libpq has PQescapeIdentifier() */
#define PQXX_HAVE_PQESCAPEIDENTIFIER 1

/* Define if libpq has PQescapeLiteral() */
#define PQXX_HAVE_PQESCAPELITERAL 1

/* Define if libpq has PQisthreadsafe() */
#define PQXX_HAVE_PQISTHREADSAFE 1

/* Define if libpq has PQmblen() */
#define PQXX_HAVE_PQMBLEN 1

/* Define if PQunescapeBytea() supports hex escape format */
#define PQXX_HAVE_PQUNESCAPEBYTEA_9 1

/* Define if std::numeric_limits has quiet_NaN() */
#define PQXX_HAVE_QUIET_NAN 1

/* Define if reverse_iterator template works as expected */
#define PQXX_HAVE_REVERSE_ITERATOR 1

/* Define if compiler has shared_ptr */
#define PQXX_HAVE_SHARED_PTR 1

/* Define if POSIX sleep() exists */
#define PQXX_HAVE_SLEEP 1

/* Define if you have the <streambuf> header; if not, <streambuf.h> is used */
#define PQXX_HAVE_STREAMBUF 1

/* Define if strerror_r exists */
#define PQXX_HAVE_STRERROR_R 1

/* Define for GNU-style strerror_r */
#define PQXX_HAVE_STRERROR_R_GNU 1

/* Define if string class has the clear() function */
#define PQXX_HAVE_STRING_CLEAR 1

/* Define if strlcpy exists */
/* #undef PQXX_HAVE_STRLCPY */

/* Define if strnlen exists */
#define PQXX_HAVE_STRNLEN 1

/* Define if the sys/select.h header defines a working select() and friends,
   as per POSIX 1003.1-2001 */
#define PQXX_HAVE_SYS_SELECT_H 1

/* Define if compiler has unique_ptr */
/* #undef PQXX_HAVE_UNIQUE_PTR */

/* Define if select() accepts NULL fd_set arguments */
#define PQXX_SELECT_ACCEPTS_NULL 1

/* Define if headers for TR1 extension to standard live in tr1/ */
#define PQXX_TR1_HEADERS 1

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "4.0"
