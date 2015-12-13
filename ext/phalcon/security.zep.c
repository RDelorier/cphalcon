
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/filter.h"
#include "kernel/fcall.h"
#include "kernel/string.h"
#include "kernel/concat.h"


/**
 * Phalcon\Security
 *
 * This component provides a set of functions to improve the security in Phalcon applications
 *
 *<code>
 *	$login = $this->request->getPost('login');
 *	$password = $this->request->getPost('password');
 *
 *	$user = Users::findFirstByLogin($login);
 *	if ($user) {
 *		if ($this->security->checkHash($password, $user->password)) {
 *			//The password is valid
 *		}
 *	}
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Security) {

	ZEPHIR_REGISTER_CLASS(Phalcon, Security, phalcon, security, phalcon_security_method_entry, 0);

	zend_declare_property_null(phalcon_security_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_long(phalcon_security_ce, SL("_workFactor"), 8, ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_long(phalcon_security_ce, SL("_numberBytes"), 16, ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_string(phalcon_security_ce, SL("_tokenKeySessionID"), "$PHALCON/CSRF/KEY$", ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_string(phalcon_security_ce, SL("_tokenValueSessionID"), "$PHALCON/CSRF$", ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_security_ce, SL("_csrf"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(phalcon_security_ce, SL("_defaultHash"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_class_constant_long(phalcon_security_ce, SL("CRYPT_DEFAULT"), 0 TSRMLS_CC);

	zend_declare_class_constant_long(phalcon_security_ce, SL("CRYPT_STD_DES"), 1 TSRMLS_CC);

	zend_declare_class_constant_long(phalcon_security_ce, SL("CRYPT_EXT_DES"), 2 TSRMLS_CC);

	zend_declare_class_constant_long(phalcon_security_ce, SL("CRYPT_MD5"), 3 TSRMLS_CC);

	zend_declare_class_constant_long(phalcon_security_ce, SL("CRYPT_BLOWFISH"), 4 TSRMLS_CC);

	zend_declare_class_constant_long(phalcon_security_ce, SL("CRYPT_BLOWFISH_X"), 5 TSRMLS_CC);

	zend_declare_class_constant_long(phalcon_security_ce, SL("CRYPT_BLOWFISH_Y"), 6 TSRMLS_CC);

	zend_declare_class_constant_long(phalcon_security_ce, SL("CRYPT_SHA256"), 7 TSRMLS_CC);

	zend_declare_class_constant_long(phalcon_security_ce, SL("CRYPT_SHA512"), 8 TSRMLS_CC);

	zend_class_implements(phalcon_security_ce TSRMLS_CC, 1, phalcon_di_injectionawareinterface_ce);
	return SUCCESS;

}

PHP_METHOD(Phalcon_Security, setWorkFactor) {

	zval *workFactor;

	zephir_fetch_params(0, 1, 0, &workFactor);



	zephir_update_property_this(this_ptr, SL("_workFactor"), workFactor TSRMLS_CC);

}

PHP_METHOD(Phalcon_Security, getWorkFactor) {

	

	RETURN_MEMBER(this_ptr, "_workFactor");

}

/**
 * Sets the dependency injector
 */
PHP_METHOD(Phalcon_Security, setDI) {

	zval *dependencyInjector;

	zephir_fetch_params(0, 1, 0, &dependencyInjector);



	zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);

}

/**
 * Returns the internal dependency injector
 */
PHP_METHOD(Phalcon_Security, getDI) {

	

	RETURN_MEMBER(this_ptr, "_dependencyInjector");

}

/**
 * Sets a number of bytes to be generated by the openssl pseudo random generator
 */
PHP_METHOD(Phalcon_Security, setRandomBytes) {

	zval *randomBytes_param = NULL, *_0;
	long randomBytes;

	zephir_fetch_params(0, 1, 0, &randomBytes_param);

	if (unlikely(Z_TYPE_P(randomBytes_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'randomBytes' must be a long") TSRMLS_CC);
		RETURN_NULL();
	}
	randomBytes = Z_LVAL_P(randomBytes_param);


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, randomBytes);
	zephir_update_property_this(this_ptr, SL("_numberBytes"), _0 TSRMLS_CC);

}

/**
 * Returns a number of bytes to be generated by the openssl pseudo random generator
 */
PHP_METHOD(Phalcon_Security, getRandomBytes) {

	

	RETURN_MEMBER(this_ptr, "_numberBytes");

}

/**
 * Generate a >22-length pseudo random string to be used as salt for passwords
 */
PHP_METHOD(Phalcon_Security, getSaltBytes) {

	zephir_fcall_cache_entry *_3 = NULL, *_5 = NULL;
	zval *numberBytes_param = NULL, *safeBytes = NULL, *_0$$4, *_1$$5 = NULL, *_2$$5 = NULL, *_4$$5 = NULL;
	int numberBytes, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &numberBytes_param);

	if (!numberBytes_param) {
		numberBytes = 0;
	} else {
		numberBytes = zephir_get_intval(numberBytes_param);
	}


	if (!((zephir_function_exists_ex(SS("openssl_random_pseudo_bytes") TSRMLS_CC) == SUCCESS))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "Openssl extension must be loaded", "phalcon/security.zep", 119);
		return;
	}
	if (!(numberBytes)) {
		ZEPHIR_OBS_VAR(_0$$4);
		zephir_read_property_this(&_0$$4, this_ptr, SL("_numberBytes"), PH_NOISY_CC);
		numberBytes = zephir_get_intval(_0$$4);
	}
	while (1) {
		ZEPHIR_INIT_NVAR(safeBytes);
		ZEPHIR_INIT_NVAR(_1$$5);
		ZVAL_LONG(_1$$5, numberBytes);
		ZEPHIR_CALL_FUNCTION(&_2$$5, "openssl_random_pseudo_bytes", &_3, 398, _1$$5);
		zephir_check_call_status();
		ZEPHIR_CALL_FUNCTION(&_4$$5, "base64_encode", &_5, 115, _2$$5);
		zephir_check_call_status();
		zephir_filter_alphanum(safeBytes, _4$$5);
		if (!(zephir_is_true(safeBytes))) {
			continue;
		}
		if (zephir_fast_strlen_ev(safeBytes) < numberBytes) {
			continue;
		}
		break;
	}
	RETURN_CCTOR(safeBytes);

}

/**
 * Creates a password hash using bcrypt with a pseudo random salt
 */
PHP_METHOD(Phalcon_Security, hash) {

	zephir_fcall_cache_entry *_3 = NULL, *_6 = NULL;
	int workFactor, ZEPHIR_LAST_CALL_STATUS, hash = 0;
	zval *password_param = NULL, *workFactor_param = NULL, *saltBytes = NULL, *_1, *_0$$3, *_2$$9, *_4$$11, *_5$$11, *_7$$13, _8$$13, _9$$13, *_10$$13 = NULL, *_11$$13;
	zval *password = NULL, *variant = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &password_param, &workFactor_param);

	zephir_get_strval(password, password_param);
	if (!workFactor_param) {
		workFactor = 0;
	} else {
		workFactor = zephir_get_intval(workFactor_param);
	}


	if (!(workFactor)) {
		ZEPHIR_OBS_VAR(_0$$3);
		zephir_read_property_this(&_0$$3, this_ptr, SL("_workFactor"), PH_NOISY_CC);
		workFactor = zephir_get_intval(_0$$3);
	}
	ZEPHIR_OBS_VAR(_1);
	zephir_read_property_this(&_1, this_ptr, SL("_defaultHash"), PH_NOISY_CC);
	hash = zephir_get_intval(_1);
	do {
		if (hash == 5) {
			ZEPHIR_INIT_VAR(variant);
			ZVAL_STRING(variant, "x", 1);
			break;
		}
		if (hash == 6) {
			ZEPHIR_INIT_NVAR(variant);
			ZVAL_STRING(variant, "y", 1);
			break;
		}
		if (hash == 7) {
			ZEPHIR_INIT_NVAR(variant);
			ZVAL_STRING(variant, "5", 1);
			break;
		}
		if (hash == 8) {
			ZEPHIR_INIT_NVAR(variant);
			ZVAL_STRING(variant, "6", 1);
			break;
		}
		ZEPHIR_INIT_NVAR(variant);
		ZVAL_STRING(variant, "a", 1);
		break;
	} while(0);

	do {
		if (hash == 1) {
			ZEPHIR_INIT_VAR(_2$$9);
			ZVAL_LONG(_2$$9, 2);
			ZEPHIR_CALL_METHOD(&saltBytes, this_ptr, "getsaltbytes", &_3, 0, _2$$9);
			zephir_check_call_status();
			if (Z_TYPE_P(saltBytes) != IS_STRING) {
				ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "Unable to get random bytes for the salt", "phalcon/security.zep", 195);
				return;
			}
			break;
		}
		if (hash == 8) {
			ZEPHIR_INIT_VAR(_4$$11);
			ZVAL_LONG(_4$$11, 8);
			ZEPHIR_CALL_METHOD(&saltBytes, this_ptr, "getsaltbytes", &_3, 0, _4$$11);
			zephir_check_call_status();
			if (Z_TYPE_P(saltBytes) != IS_STRING) {
				ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "Unable to get random bytes for the salt", "phalcon/security.zep", 203);
				return;
			}
			ZEPHIR_INIT_VAR(_5$$11);
			ZEPHIR_CONCAT_SVSV(_5$$11, "$", variant, "$", saltBytes);
			ZEPHIR_RETURN_CALL_FUNCTION("crypt", &_6, 399, password, _5$$11);
			zephir_check_call_status();
			RETURN_MM();
		}
		ZEPHIR_INIT_VAR(_7$$13);
		ZVAL_LONG(_7$$13, 22);
		ZEPHIR_CALL_METHOD(&saltBytes, this_ptr, "getsaltbytes", &_3, 0, _7$$13);
		zephir_check_call_status();
		if (Z_TYPE_P(saltBytes) != IS_STRING) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "Unable to get random bytes for the salt", "phalcon/security.zep", 225);
			return;
		}
		if (workFactor < 4) {
			workFactor = 4;
		} else {
			if (workFactor > 31) {
				workFactor = 31;
			}
		}
		ZEPHIR_SINIT_VAR(_8$$13);
		ZVAL_STRING(&_8$$13, "%02s", 0);
		ZEPHIR_SINIT_VAR(_9$$13);
		ZVAL_LONG(&_9$$13, workFactor);
		ZEPHIR_CALL_FUNCTION(&_10$$13, "sprintf", NULL, 188, &_8$$13, &_9$$13);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_11$$13);
		ZEPHIR_CONCAT_SVSVSV(_11$$13, "$2", variant, "$", _10$$13, "$", saltBytes);
		ZEPHIR_RETURN_CALL_FUNCTION("crypt", &_6, 399, password, _11$$13);
		zephir_check_call_status();
		RETURN_MM();
	} while(0);

	RETURN_MM_STRING("", 1);

}

/**
 * Checks a plain text password and its hash version to check if the password matches
 */
PHP_METHOD(Phalcon_Security, checkHash) {

	unsigned char _4$$5;
	zend_bool _0$$3;
	long _3;
	char ch = 0;
	int maxPassLength, ZEPHIR_LAST_CALL_STATUS, i = 0, sum = 0, cryptedLength = 0, passwordLength = 0;
	zval *password_param = NULL, *passwordHash_param = NULL, *maxPassLength_param = NULL, *_1 = NULL;
	zval *password = NULL, *passwordHash = NULL, *cryptedHash = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &password_param, &passwordHash_param, &maxPassLength_param);

	zephir_get_strval(password, password_param);
	zephir_get_strval(passwordHash, passwordHash_param);
	if (!maxPassLength_param) {
		maxPassLength = 0;
	} else {
		maxPassLength = zephir_get_intval(maxPassLength_param);
	}


	if (maxPassLength) {
		_0$$3 = maxPassLength > 0;
		if (_0$$3) {
			_0$$3 = zephir_fast_strlen_ev(password) > maxPassLength;
		}
		if (_0$$3) {
			RETURN_MM_BOOL(0);
		}
	}
	ZEPHIR_CALL_FUNCTION(&_1, "crypt", NULL, 399, password, passwordHash);
	zephir_check_call_status();
	zephir_get_strval(_2, _1);
	ZEPHIR_CPY_WRT(cryptedHash, _2);
	cryptedLength = zephir_fast_strlen_ev(cryptedHash);
	passwordLength = zephir_fast_strlen_ev(passwordHash);
	zephir_concat_self(&cryptedHash, passwordHash TSRMLS_CC);
	sum = (cryptedLength - passwordLength);
	for (_3 = 0; _3 < Z_STRLEN_P(passwordHash); _3++) {
		i = _3; 
		ch = ZEPHIR_STRING_OFFSET(passwordHash, _3);
		_4$$5 = ZEPHIR_STRING_OFFSET(cryptedHash, i);
		sum = (sum | ((_4$$5 ^ ch)));
	}
	RETURN_MM_BOOL(0 == sum);

}

/**
 * Checks if a password hash is a valid bcrypt's hash
 */
PHP_METHOD(Phalcon_Security, isLegacyHash) {

	zval *passwordHash_param = NULL;
	zval *passwordHash = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &passwordHash_param);

	zephir_get_strval(passwordHash, passwordHash_param);


	RETURN_MM_BOOL(zephir_start_with_str(passwordHash, SL("$2a$")));

}

/**
 * Generates a pseudo random token key to be used as input's name in a CSRF check
 */
PHP_METHOD(Phalcon_Security, getTokenKey) {

	zval *numberBytes_param = NULL, *safeBytes = NULL, *dependencyInjector = NULL, *session = NULL, *_0, *_1 = NULL, *_2 = NULL, *_3 = NULL, *_4 = NULL;
	int numberBytes, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &numberBytes_param);

	if (!numberBytes_param) {
		numberBytes = 0;
	} else {
		numberBytes = zephir_get_intval(numberBytes_param);
	}


	if (!(numberBytes)) {
		numberBytes = 12;
	}
	if (!((zephir_function_exists_ex(SS("openssl_random_pseudo_bytes") TSRMLS_CC) == SUCCESS))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "Openssl extension must be loaded", "phalcon/security.zep", 292);
		return;
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(dependencyInjector, _0);
	if (Z_TYPE_P(dependencyInjector) != IS_OBJECT) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "A dependency injection container is required to access the 'session' service", "phalcon/security.zep", 297);
		return;
	}
	ZEPHIR_INIT_VAR(safeBytes);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, numberBytes);
	ZEPHIR_CALL_FUNCTION(&_2, "openssl_random_pseudo_bytes", NULL, 398, _1);
	zephir_check_call_status();
	ZEPHIR_CALL_FUNCTION(&_3, "base64_encode", NULL, 115, _2);
	zephir_check_call_status();
	zephir_filter_alphanum(safeBytes, _3);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_STRING(_1, "session", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_METHOD(&_4, dependencyInjector, "getshared", NULL, 0, _1);
	zephir_check_temp_parameter(_1);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(session, _4);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_tokenKeySessionID"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(NULL, session, "set", NULL, 0, _0, safeBytes);
	zephir_check_call_status();
	RETURN_CCTOR(safeBytes);

}

/**
 * Generates a pseudo random token value to be used as input's value in a CSRF check
 */
PHP_METHOD(Phalcon_Security, getToken) {

	zval *numberBytes_param = NULL, *token = NULL, *dependencyInjector = NULL, *session = NULL, *_0 = NULL, *_1 = NULL, *_2;
	int numberBytes, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &numberBytes_param);

	if (!numberBytes_param) {
		numberBytes = 0;
	} else {
		numberBytes = zephir_get_intval(numberBytes_param);
	}


	if (!(numberBytes)) {
		numberBytes = 12;
	}
	if (!((zephir_function_exists_ex(SS("openssl_random_pseudo_bytes") TSRMLS_CC) == SUCCESS))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "Openssl extension must be loaded", "phalcon/security.zep", 319);
		return;
	}
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, numberBytes);
	ZEPHIR_CALL_FUNCTION(&token, "openssl_random_pseudo_bytes", NULL, 398, _0);
	zephir_check_call_status();
	ZEPHIR_CALL_FUNCTION(&_1, "base64_encode", NULL, 115, token);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(token, _1);
	ZEPHIR_INIT_NVAR(_0);
	zephir_filter_alphanum(_0, token);
	ZEPHIR_CPY_WRT(token, _0);
	_2 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(dependencyInjector, _2);
	if (Z_TYPE_P(dependencyInjector) != IS_OBJECT) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "A dependency injection container is required to access the 'session' service", "phalcon/security.zep", 329);
		return;
	}
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "session", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_METHOD(&_1, dependencyInjector, "getshared", NULL, 0, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(session, _1);
	_2 = zephir_fetch_nproperty_this(this_ptr, SL("_tokenValueSessionID"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(NULL, session, "set", NULL, 0, _2, token);
	zephir_check_call_status();
	RETURN_CCTOR(token);

}

/**
 * Check if the CSRF token sent in the request is the same that the current in session
 */
PHP_METHOD(Phalcon_Security, checkToken) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool destroyIfValid, returnValue = 0, _5;
	zval *tokenKey = NULL, *tokenValue = NULL, *destroyIfValid_param = NULL, *dependencyInjector = NULL, *session = NULL, *request = NULL, *token = NULL, *_0, *_1 = NULL, *_2, *_3$$4, *_4$$6, *_6$$8, *_7$$8;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 3, &tokenKey, &tokenValue, &destroyIfValid_param);

	if (!tokenKey) {
		ZEPHIR_CPY_WRT(tokenKey, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(tokenKey);
	}
	if (!tokenValue) {
		tokenValue = ZEPHIR_GLOBAL(global_null);
	}
	if (!destroyIfValid_param) {
		destroyIfValid = 1;
	} else {
		destroyIfValid = zephir_get_boolval(destroyIfValid_param);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(dependencyInjector, _0);
	if (Z_TYPE_P(dependencyInjector) != IS_OBJECT) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "A dependency injection container is required to access the 'session' service", "phalcon/security.zep", 348);
		return;
	}
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "session", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_METHOD(&_1, dependencyInjector, "getshared", NULL, 0, _2);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(session, _1);
	if (!(zephir_is_true(tokenKey))) {
		_3$$4 = zephir_fetch_nproperty_this(this_ptr, SL("_tokenKeySessionID"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&tokenKey, session, "get", NULL, 0, _3$$4);
		zephir_check_call_status();
	}
	if (!(zephir_is_true(tokenKey))) {
		RETURN_MM_BOOL(0);
	}
	if (!(zephir_is_true(tokenValue))) {
		ZEPHIR_INIT_VAR(_4$$6);
		ZVAL_STRING(_4$$6, "request", ZEPHIR_TEMP_PARAM_COPY);
		ZEPHIR_CALL_METHOD(&request, dependencyInjector, "getshared", NULL, 0, _4$$6);
		zephir_check_temp_parameter(_4$$6);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&token, request, "getpost", NULL, 0, tokenKey);
		zephir_check_call_status();
	} else {
		ZEPHIR_CPY_WRT(token, tokenValue);
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_tokenValueSessionID"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_1, session, "get", NULL, 0, _0);
	zephir_check_call_status();
	returnValue = ZEPHIR_IS_EQUAL(token, _1);
	_5 = returnValue;
	if (_5) {
		_5 = destroyIfValid;
	}
	if (_5) {
		_6$$8 = zephir_fetch_nproperty_this(this_ptr, SL("_tokenKeySessionID"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, session, "remove", NULL, 0, _6$$8);
		zephir_check_call_status();
		_7$$8 = zephir_fetch_nproperty_this(this_ptr, SL("_tokenValueSessionID"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, session, "remove", NULL, 0, _7$$8);
		zephir_check_call_status();
	}
	RETURN_MM_BOOL(returnValue);

}

/**
 * Returns the value of the CSRF token in session
 */
PHP_METHOD(Phalcon_Security, getSessionToken) {

	zval *dependencyInjector = NULL, *session = NULL, *_0, *_1 = NULL, *_2;
	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(dependencyInjector, _0);
	if (Z_TYPE_P(dependencyInjector) != IS_OBJECT) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "A dependency injection container is required to access the 'session' service", "phalcon/security.zep", 401);
		return;
	}
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "session", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_METHOD(&_1, dependencyInjector, "getshared", NULL, 0, _2);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(session, _1);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_tokenValueSessionID"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_METHOD(session, "get", NULL, 0, _0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Removes the value of the CSRF token and key from session
 */
PHP_METHOD(Phalcon_Security, destroyToken) {

	zval *dependencyInjector = NULL, *session = NULL, *_0, *_1 = NULL, *_2, *_3;
	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(dependencyInjector, _0);
	if (Z_TYPE_P(dependencyInjector) != IS_OBJECT) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_security_exception_ce, "A dependency injection container is required to access the 'session' service", "phalcon/security.zep", 418);
		return;
	}
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "session", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_METHOD(&_1, dependencyInjector, "getshared", NULL, 0, _2);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(session, _1);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_tokenKeySessionID"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(NULL, session, "remove", NULL, 0, _0);
	zephir_check_call_status();
	_3 = zephir_fetch_nproperty_this(this_ptr, SL("_tokenValueSessionID"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(NULL, session, "remove", NULL, 0, _3);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

/**
 * Computes a HMAC
 *
 * @param string data
 * @param string key
 * @param string algo
 * @param boolean raw
 */
PHP_METHOD(Phalcon_Security, computeHmac) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool raw;
	zval *data, *key, *algo, *raw_param = NULL, *hmac = NULL, _0, *_1$$3, *_2$$3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 1, &data, &key, &algo, &raw_param);

	if (!raw_param) {
		raw = 0;
	} else {
		raw = zephir_get_boolval(raw_param);
	}


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_BOOL(&_0, (raw ? 1 : 0));
	ZEPHIR_CALL_FUNCTION(&hmac, "hash_hmac", NULL, 400, algo, data, key, &_0);
	zephir_check_call_status();
	if (!(zephir_is_true(hmac))) {
		ZEPHIR_INIT_VAR(_1$$3);
		object_init_ex(_1$$3, phalcon_security_exception_ce);
		ZEPHIR_INIT_VAR(_2$$3);
		ZEPHIR_CONCAT_SV(_2$$3, "Unknown hashing algorithm: %s", algo);
		ZEPHIR_CALL_METHOD(NULL, _1$$3, "__construct", NULL, 9, _2$$3);
		zephir_check_call_status();
		zephir_throw_exception_debug(_1$$3, "phalcon/security.zep", 441 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(hmac);

}

/**
 * Sets the default hash
 */
PHP_METHOD(Phalcon_Security, setDefaultHash) {

	zval *defaultHash;

	zephir_fetch_params(0, 1, 0, &defaultHash);



	zephir_update_property_this(this_ptr, SL("_defaultHash"), defaultHash TSRMLS_CC);

}

/**
 * Sets the default hash
 */
PHP_METHOD(Phalcon_Security, getDefaultHash) {

	

	RETURN_MEMBER(this_ptr, "_defaultHash");

}

