<?php

# %BEGINCOPYRIGHT%
# Copyright Ian McWilliam (kaosagnt@gmail.com).
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
# IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
# OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
# %ENDCOPYRIGHT%

# A CGI class modelled and cut down based upon
# the Perl CGI.pm / CGI::minimal concepts.
# This class does not contain any of the HTML elements for
# creating HTML, Header props and the like. It will be
# expanded with functionality as required.

class CGI {

	private $_REQUEST_METHOD = NULL;
	private $_SERVER_PORT = NULL;
	private $_SERVER_PROTOCOL = NULL;
	private $_PARAMS;
	private $_URL = '';

	public function __construct($cgi_arg = NULL) {

		# %PUBLIC%
		# NAME:*		__construct()
		# DESCR:*		Create the CGI object class.
		# USAGE:*		$CGI_object = new CGI();
		# RETURNS:*		Returns reference the the created class.
		# %END_PUBLIC%

		# At present you can not pass a CGI object to fill set defaults.
		# This functionality will be added later.

		# Work out the CGI request method and pre-fill a bunch of param names and values
		# based upon the CGI environment.
		# These are referenced in the $CGI_object->param('xyz') manner.
		# They can also be modified by setting $CGI_object->param('xyz', 'some value');
		# XXX TODO: add support of passing in a reference to an existing CGI object and
		# clone it's values and params XXX

		if (is_scalar($cgi_arg) && $cgi_arg != NULL) {

			# We have a URL to decode
			$this->_URL = $this->unescapeHTML($cgi_arg);

			if (preg_match('/https/i', $this->_URL)) {
				$this->_SERVER_PROTOCOL = 'https';

			} else {

				$this->_SERVER_PROTOCOL = 'http';
			}

			if (preg_match('/:\d\//i', $this->_URL, $temp_port)) {
				$this->_SERVER_PORT = $temp_port[0];

			} else {

				if ($this->_SERVER_PROTOCOL == 'https') {
					$this->_SERVER_PORT = 443;

				} else {

					$this->_SERVER_PORT = 80;
				}
			}

			list($URI, $query_params_string) = explode('?', $this->_URL);

			if (is_set($param_array_string)) {

				# We have a string of something to digest.
				$param_array_string = preg_split('/[&;]/', $query_params_string);

				foreach (explode('=', $query_params_string) as $key => $value) {
					if (! empty($value) && ! empty($key)) {
						$this->param($key, $value);
					}
				}
			}

		} else {

			# Not passed a URL or CGI object Gather params from environment.
			# XXX Need to add stuff for storing the URL XXX

			if (! empty($_SERVER['SERVER_PORT'])) {
				# Set the $_SERVER_PORT
				$this->_SERVER_PORT = $_SERVER['SERVER_PORT'];
			}

			if (! empty($_SERVER['REQUEST_METHOD'])) {
				$this->_REQUEST_METHOD = $_SERVER['REQUEST_METHOD'];
			}

			if (! empty($_SERVER['HTTPS']) && preg_match('/on/i', $_SERVER['HTTPS'])) {
				$this->_SERVER_PROTOCOL = 'https';

			} else {

				$this->_SERVER_PROTOCOL = 'http';
			}

			switch($this->_REQUEST_METHOD) {
				case 'GET':
					# Grab our GET elements from $_GET
					# and add to $this->param('xyz', 'value');
					$this->param($_GET);

					break;

				case 'POST':
					# Grab our POST elements from $_POST
					# and add to $this->param('xyz', 'value');
					$this->param($_POST);

					break;

				default:
					# Nothing to set
					break;
			}
		}

		# Debug
		#error_log(Dumper(self::param()));

		# URL decode the query parameter values.
		foreach(self::param() as $parameter_key => $parameter_set) {

			# Debug
			#error_log('$parameter_key = ' . Dumper($parameter_key));

			if (is_array($parameter_set)) {

				$temp_array = array();

				foreach($parameter_set as $array_parameter_key => $array_parameter_set) {

					# Debug
					#error_log('Array $parameter_set = ' . Dumper($parameter_set));
					#error_log('Array $array_parameter_key = ' . Dumper($array_parameter_key));
					#error_log('Array $array_parameter_set = ' . Dumper($array_parameter_set));

					if (is_array($array_parameter_set)) {

						$temp_array[] = $array_parameter_set;

					} else {

						$temp_array[] = urldecode($array_parameter_set);
					}
				}

				self::param($parameter_key, $temp_array);

			} else {

				# Debug
				#error_log(
				#	'Not array $parameter_key = ' . $parameter_key .
				#	' $parameter_set = ' . $parameter_set
				#);

				self::param($parameter_key, urldecode($parameter_set));
			}
		}

		# Debug
		#error_log(Dumper(self::param()));

		return(true);
	}

	final public function request_method() {

		# %PUBLIC%
		# NAME:*		request_method
		# DESCR:*		Find out the CGI page request method and return it.
		# USAGE:*		$request_method = $CGI_object->request_method();
		# RETURNS:*		Returns string containing the CGI request method. eg 'POST' or 'GET'.
		# %END_PUBLIC%

		# Work out our request method
		return($this->_REQUEST_METHOD);
	}

	final public function protocol() {

		# %PUBLIC%
		# NAME:*		protocol
		# DESCR:*		Find out the HTTP/HTTPS protocol used and return it.
		# USAGE:*		$request_protocol = $CGI_object->protocol();
		# RETURNS:*		Returns string containing the protocol. eg 'http' or 'https'.
		# %END_PUBLIC%

		return($this->_SERVER_PROTOCOL);
	}

	final public function Vars() {

		# %PUBLIC%
		# NAME:*		Vars
		# DESCR:*		Return the names and values of the current CGI elements.
		# USAGE:*		$CGI_parameter_names = $CGI_object->Vars();
		# RETURNS:*		Associative array of CGI element names and values.
		# %END_PUBLIC%

		# This is just a wrapper around the param() function.
		# The param() function when called without an argument will return
		# an associative array of names and values.
		return(self::param());
	}

	final public function append($param_name, $param_value) {

		# %PUBLIC%
		# NAME:*		append
		# DESCR:*		Append the passed param name and value to the list
		# :*			of CGI Query params.
		# USAGE:*		$CGI_object->append($param_name, $param_value);
		# RETURNS:*		Returns true.
		# %END_PUBLIC%

		# This is just a wrapper around the param() function.
		# If the param does not exist.

		# XXX Need to look up what CGI.pm does here XXX

		return(self::param($param_name, $param_value));
	}

	final public function escapeHTML($string = '') {

		# %PUBLIC%
		# NAME:*		escapeHTML
		# DESCR:*		Given a string, escape any characters into
		# :*			HTML entity equivalents.
		# USAGE:*		$CGI_object->escapeHTML($string);
		# RETURNS:*		String with characters converted into HTML Entities.
		# %END_PUBLIC%

		return(htmlentities($string, ENT_QUOTES));
	}

	final public function unescapeHTML($string = '') {

		# %PUBLIC%
		# NAME:*		unescapeHTML
		# DESCR:*		Given a string containing escape HTML Entities,
		# :* 			unescape the HTML Entities into their original form.
		# USAGE:*		$CGI_object->unscapeHTML($string);
		# RETURNS:*		Unescaped string of characters.
		# %END_PUBLIC%

		return(html_entity_decode($string, ENT_QUOTES));
	}

    final public function param() {

		# %PUBLIC%
		# NAME:*		param
		# DESCR:*		Store / Retrieve variable data associated with the calling class.
		# USAGE:*		$CGI_object->param() - Retrieve all parameters keys / values set.
		# :*			$CGI_object->param('xyz'); - Retrieve a parameter (element) value
		# :*			$CGI_object->param('xyz', 'some_value'); - Set a parameter (element) value
		# :*			$CGI_object->param(
		# :*				array(
		# :*					'xyz' => 'some_value',
		# :*					'yyz' => 'another_value'
		# :*				)
		# :*			); - Set multiple parameters (elements) values
		# RETURNS:*		True on success on setting parameter(s).
		# :*			The param value requested when retrieving.
		# :*			An associative array of all parameters when no args are passed.
		# :*			Will die() if passed odd number of arguments if sent an associative array.
		# %END_PUBLIC%

		# Create new _PARAMS
		if (!isset($this->_PARAMS)) {
			$this->_PARAMS = array();
		}

		$params_array = $this->_PARAMS;

		# Determine how many arguments were sent
		$number_of_args = func_num_args();

		if ($number_of_args == 0) {
			# If no values passed, return the list of parameters
			$params = array();

			foreach($params_array as $index => $param) {
				$params[strtolower($index)] = $this->$index;
			}

			return($params);

		} elseif ($number_of_args == 1) {

			# One argument passed to param; grab it.
			$data = func_get_arg(0);

			if (is_string($data)) {
				# Argument is a scalar
				$data = strtoupper(trim($data));

				if (isset($params_array[$data])) {
					# Argument is in parameter list; return it
					return($this->$data);
				}

				# Argument is not in parameter list; return false
				return(false);

			} elseif (!is_array($data)) {
				# Object sent to param; error!
				die('Bad arguments (object) sent to param()');

			} elseif (!self::_is_assoc_array($data) && (0 == (count($data) % 2))) {
				# Even-indexed array passed to method; turn it into a hash
				$data = self::_array_to_hash($data);

			} elseif (!self::_is_assoc_array($data) && (0 != (count($data) % 2))) {
				# Odd-indexed array passed to method -- error!
				die('Bad arguments (array) sent to param()');
			}

			# At this point, we know we have an associative array.
			# Initialise each parameter in it and add it to our parameter list
			$params = array();

			foreach ($data as $key => $val) {
				$key = strtoupper(trim($key));	# Normalise the key
				$this->$key = $val;				# Set the property
				$params[$key] = true;			# Store the param for later
			}

			$params_array = array_merge($params_array, $params);

		} elseif ($number_of_args == 2) {
			# Two arguments passed
			$key = func_get_arg(0);

			if (!is_string($key)) {
				# Non-scalar key sent -- error
				die('Bad key (non-string) sent to param()');
				return;
			}

			$key = strtoupper(trim($key));
			$this->$key = func_get_arg(1);
			$params_array = array_merge($params_array, array($key => true));

		} else {

			# Two many arguments sent to param.
			die('Too many arguments sent to param()');
		}

		$this->_PARAMS = $params_array;
		return(true);
	}

	final public function delete($key = NULL) {

		# %PUBLIC%
		# NAME:*		delete
		# DESCR:*		Delete the CGI parameter (element) name value pairs associated with this CGI
		# :*			object for the key passed.
		# USAGE:*		$CGI_object->delete($param_name);
		# RETURNS:*		Nothing. $CGI_object->param($param_name) parameter (element) is deleted.
		# %END_PUBLIC%

		# Is the _PARAMS property set? if not, return
		# Is our $key value set?
		if (!isset($this->_PARAMS) || !is_string($key) || $key == '' || $key == NULL) {
			return(false);
		}

		$key = strtoupper(trim($key));
		if (isset($this->_PARAMS[$key])) {
			unset($this->_PARAMS[$key]);
			unset($this->$key);
			return(true);
		}

		return(false);
	}

	final public function delete_all() {

		# %PUBLIC%
		# NAME:*		delete_all
		# DESCR:*		Delete ALL CGI parameter (element) name value pairs associated with this CGI
		# :*			object.
		# USAGE:*		$CGI_object->delete_all();
		# RETURNS:*		Nothing. All $CGI_object->param('xyz') parameters (elements) are deleted.
		# %END_PUBLIC%

		# Get the param list and delete.
		foreach($this->_PARAMS as $parameter_key => $parameter_set) {
			self::delete($parameter_key);
		}

		return;
	}

	final public function cookies() {

		# %PUBLIC%
		# NAME:*		cookies
		# DESCR:*		Fetch the names of all cookies and return them
		# USAGE:*		$CGI_object->cookies();
		# RETURNS:*		Array of cookie names or NULL if no cookies found.
		# %END_PUBLIC%

		$cookie_count = count($_COOKIE);

		if ($cookie_count > 0) {

			$cookie_name_array = array();

			foreach ($_COOKIE as $cookie_name => $cookie_value) {
				$cookie_name_array[] = $cookie_name;
			}

			# We have cookies
			return($cookie_name_array);
		}

		# No Cookies
		return(NULL);
	}
#-----------------------------
# Private functions
#-----------------------------
	private static function _is_assoc_array($php_val = NULL) {

		# %PRIVATE%
		# NAME:*		_is_assoc_array
		# DESCR:*		Given an array, determine whether it is an
		# :*			associative array or not.
		# USAGE:*		$is_assoc_array = self::_is_assoc_array($array);
		# RETURNS:*		Returns false if not associative array or true.
		# %END_PRIVATE%

		if (!is_array($php_val)) {
			# Neither an associative, nor non-associative array.
			return(false);
		}

		return(array_keys($php_val) !== range(0, count($php_val) - 1));
	}

	private static function _array_to_hash($assoc_array = NULL) {

		# %PRIVATE%
		# NAME:*		array_to_hash
		# DESCR:*		Given an indexed array with an even number of elements, create
		# :*			an associative array out of the index array.
		# USAGE:*		$new_hash_array = self::_array_to_hash($array);
		# RETURNS:*		Returns associative array.
		# %END_PRIVATE%

		if (!is_array($assoc_array) || self::_is_assoc_array($assoc_array)
			|| (is_array($assoc_array) && ((count($assoc_array) % 2) != 0))) {
			return(false);
		}

		$hash = array();

		for ($index = 0; $index < count($assoc_array); $index = $index + 2) {
			$key = $assoc_array[$index];

			if (is_string($key)) {
				$hash[$key] = $assoc_array[$index + 1];
			}
		}

		return($hash);
	}

} # End CGI class.

?>