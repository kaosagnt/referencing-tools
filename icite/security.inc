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

function filter_nasties($string_to_filter = '', $filter_type = '', $default =  '') {

	# %PUBLIC%
	# NAME:*		filter_nasties
	# DESCR:*		Given a string, filter it in the same manner as secure_CGI_object().
	# :*			Filter types are the same. Giving a default value means that if the String is empty,
	# :*			it will be set to the default. The string is run against _deltag() to remove
	# :*			common XSS tags. This is not a replacement for proper HTML escaping upon display
	# :*			but to augment secure_CGI_object so we can filter strings the same way. The string
	# :*			may have been placed within a cookie, or come from the database or from an unknown source.
	# USAGE:*		$filtered_string = filter_nasties($some_string, $filter_type, $default_value);
	# :*			$filtered_string = filter_nasties($some_string, 'alpha_space', '');
	# RETURNS:*		Filtered string based upon filter name passed.
	# %END_PUBLIC%

	$string_to_filter = _filter($filter_type, $string_to_filter , $default);
	$string_to_filter = _deltag($string_to_filter);

	return($string_to_filter);
}

function secure_CGI_object($filter_hash = null) {

	# %PUBLIC%
	# NAME:*	secure_CGI_object
	# DESCR:*	Main security method to prevent XSS attacks and provide CGI filtering.
	# :*		Takes CGI parameter object and applies data defaults, filters and XSS security.
	# :*		For extra security, deletes all cgi query parameters not given in the valid filter hash (see eg below).
	# :*		List of values is: [<default value>, <filter>, 'unsafe', 'notarray']
	# :*		where:	<default value> is assigned to a new cgi parameter if not provided in the GET or POST
	# :*			<filter> is one of several filters available to filter the data.  See _filter() below for options.
	# :*			'unsafe' makes a copy of the raw (unsafe) GET or POST argument to a new parameter with '_unsafe' appended to the name
	# :*			eg. in example below, $CGI_object->param('kw_unsafe') would be created with raw (unsafe) data
	# :*			'notarray' ensures only the first (scalar) query parameter occurence is used. Multiples are deleted.
	# USAGE:*
	# :*		secure_CGI_object(
	# :*			array(
	# :*				'cgi'	=> $CGI_object,				# pointer to CGI object from CGI.class.php The target object is modified to make safe.
	# :*				'ID'	=> array(0, 'int'),				# list of valid cgi parameters with [<default value>, <filter>, ['unsafe']]
	# :*				'tp'	=> array('','filepath'),
	# :*				'kw'	=> array('', '', 'unsafe'),		# provides a second unsafe version
	# :*				'p'		=> array(1, 'boolean'),
	# :*				'page'	=> array(0, 'int'),
	# :*				'ind'	=> array(0, 'int'),
	# :*				'sah'	=> array(0, 'int', 'notarray']	# returns first single scalar query parameter only
	# :*			)
	# :*		);
	# RETURNS:*	Nothing. The Passed CGI Object is modified.
	# %END_PUBLIC%

	if ($filter_hash == null) {
		die('No CGI Filters passed to secure_CGI_object() method.');
	}

	$CGI_object = isset($filter_hash['cgi']) ? $filter_hash['cgi'] : null;

	if ($CGI_object == null) {
		die('No CGI object passed to secure_CGI_object() method.');
	}

	# Remove cgi from the filter_hash.
	unset($filter_hash['cgi']);

	# Debug
	#die(Dumper($filter_hash));

	foreach ($CGI_object->Vars() as $CGI_key => $CGI_value) {
		if (! array_key_exists($CGI_key, $filter_hash)) {
			# Remove all CGI parameters that don't appear in the caller's filter hash
			$CGI_object->delete($CGI_key);
		}
	}

	foreach ($CGI_object->Vars() as $CGI_key => $CGI_value) {
		# Where requested, create an unsafe copy of all raw CGI parameters
		if (isset($filter_hash[$CGI_key][2])) {
			if ($filter_hash[$CGI_key][2] == 'unsafe' ||
				(isset($filter_hash[$CGI_key][3]) && $filter_hash[$CGI_key][3] == 'unsafe')) {
					$CGI_object->param($CGI_key . '_unsafe', $CGI_object->param($CGI_key));
			}

			# Where requested, ensure only single (scalar) value, not array
			if (is_array($CGI_object->param($CGI_key)) &&
				($filter_hash[$CGI_key][2] == 'notarray' ||
				(isset($filter_hash[$CGI_key][3]) && $filter_hash[$CGI_key][3] == 'notarray'))) {
					$temp_data = $CGI_query->param($CGI_key);
					$CGI_object->param($CGI_key, $temp_data[0]);
			}
		}

		# Debug
		#error_log($filter_hash[$CGI_key][1]);

		# Filter all parameters according to filter hash
		$temp_filter_data = array();
		foreach (array($CGI_object->param($CGI_key)) as $data_to_strip) {
			$temp_filter_data[] = _filter($filter_hash[$CGI_key][1], $data_to_strip, $filter_hash[$CGI_key][0]);
		}

		if (count($temp_filter_data) >= 1) {

			$CGI_object->param($CGI_key, $temp_filter_data);

		} else {

			$CGI_object->param($CGI_key, $temp_filter_data[0]);
		}

		# Filter all parameters to remove XSS nasties
		$temp_tag = array();

		foreach (array($CGI_object->param($CGI_key)) as $tag_to_strip) {

			if (is_array($tag_to_strip)) {

				foreach($tag_to_strip as $data_to_strip) {

					if (is_array($data_to_strip)) {

						$temp_array = array();

						foreach($data_to_strip as $data) {

							# Debug
							#error_log('lib_common: secure_CGI_object() -> data = ' . Dumper($data));

							$temp_array[] = _deltag($data);
						}

						# Debug
						#error_log('lib_common: secure_CGI_object() -> temp_array = ' . Dumper($temp_array));

						$temp_tag[] = $temp_array;

					} else {

						# Debug
						#error_log('lib_common: secure_CGI_object() -> single strip');

						$temp_tag[] = _deltag($data_to_strip);
					}
				}

			} else {

				$temp_tag[] = _deltag($tag_to_strip);
			}

			# Debug
			#error_log('lib_common: secure_CGI_object() -> $temp_tag = ' . Dumper($temp_tag));
		}

		if (count($temp_tag) > 1) {

			$CGI_object->param($CGI_key, $temp_tag);

		} else {

			$CGI_object->param($CGI_key, $temp_tag[0]);
		}
	}

	foreach ($filter_hash as $filter_key => $filter_value) {
		if ($CGI_object->param($filter_key) === false) {
			# Add any query params not provided in the call but appear in the filter hash & set to default values
			$CGI_object->param($filter_key, $filter_value[0]);

			if (count($filter_value) >= 2 && ($filter_value[2] == 'unsafe') ||
					(isset($filter_value[3]) && $filter_value[3] == 'unsafe')) {
						# And where requested, create the unsafe (actually safe) default copy
						$CGI_object->param($filter_key . '_unsafe', $filter_value[0]);
			}
		}
	}

	# Debug
	#die(Dumper($filter_hash));
	return;
}

#----------------------------
# Private Functions
#----------------------------
function _filter($filter_type = '', $data = '', $default = '') {

	# %PRIVATE%
	# NAME:*		_filter
	# DESCR:*		private method strips unwanted chars based on type.
	# :*			Will DIE if given an unknown filter type.
	# :*			Caller should check validity.
	# USAGE:*		Internal Function - DO NOT USE.
	# RETURNS:*		Returns Filtered Input based on filter type specified.
	# %END_PRIVATE%

	# Debug
	#error_log(': Filter Type: ' . $filter_type);

	if ($filter_type == 'none' || $filter_type == '') {
		return($data);
	}

	switch($filter_type) {
		case 'int':
			# perl equiv - return $data =~ /^(\d+)/ ? $1 : $default;
			return(preg_match("/^(\d+)/", $data, $matched) ? $matched[0] : $default);

		case 'hex':
			# perl equiv - return $data =~ /^(#?(\d|[A-F])+)/i ? $1 : $default;
			return(preg_match("/^(#?(\d|[A-F][a-f])+)/", $data, $matched) ? $data : $default);

		case 'session_id':
			return(preg_match("/^([A-Z0-9]+)$/i", $data, $matched) ? $data : $default);

		case 'alpha':
			# perl equiv - $data =~ s/[^\w]//sg; return $data;
			# Allows alphanumeric plus _
			return(preg_replace("/[^\w]/", '', $data));

		case 'alpha_space':
			# perl equiv - $data =~ s/[^\w\s]//sg; return($data);
			# as above plus whitespace
			return(preg_replace("/[^\w\s]/", '', $data));

		case 'alpha_space_star':
			# perl equiv - $data =~ s/[^\w\s\*]//sg; return($data);
			# Allows alphanumeric plus whitespace *
			return(preg_replace("/[^\w\s\*]/", '', $data));

		case 'alpha_space_dot_comma':
			# perl equiv - $data =~ s/[^\w\s\.\,]//sg; return($data);
			# Allows alphanumeric plus whitespace . ,
			return(preg_replace("/[^\w\s\.\,]/", '', $data));

		case 'alpha_space_dot_comma_amp_underscore_dash':
			# perl equiv - $data =~ s/[^\w\s\.\,\&\_\-]//sg; return $data;
			# Allows alphanumeric plus whitespace . , & _
			return(preg_replace("/[^\w\s\.\,\&\_\-]/", '', $data));

		case 'environment_variables':
			# perl equiv - $data =~ s/[^\w\s\.\,\;\/\+\-\_\(\)\!\=\*\:]//sg; return($data);
			# Allows alphanumeric plus whitespace . , ; / + - _ ( ) ! * :
			return(preg_replace("/[^\w\s\.\,\;\/\+\-\_\(\)\!\=\*\:\?]/", '', $data));

		case 'alpha_csv':
			# perl equiv - $data =~ s/[^\w\s,]//sg; return($data);
			# as above plus comma
			return(preg_replace("/[^\w\s,]/", '', $data));

		case 'alpha_search_keywords':
			# perl equiv - $data =~ s/[^\w\s,\+\-]//sg; return($data);
			# allows alphanumeric plus whitespace , + - :
			return(preg_replace("/[^\w\s,\+\-\:]/", '', $data));

		case 'alpha_dash_underscore':
			# perl equiv - $data =~ s/[^\w\-\_]//sg; return($data);
			return(preg_replace("/[^\w\-\_]/", '', $data));

		case 'alpha_underscore_colon':
			# perl equiv - $data =~ s/[^\w\s\_\:]//sg; return($data);
			# Allows alphanumeric plus whitespace . ,
			return(preg_replace("/[^\w\s\_\:]/", '', $data));

		case 'boolean':
			# perl equiv - return($data =~ /^1|y|on$/ ? 1 : 0);
			return(preg_match("/^1|y|on$/", $data) ? 1 : 0);

		case 'decimal':
			# perl equiv - return($data =~ /^([\d\.]+)/ ? $1 : $default);
			return(preg_match("/^([\d\.]+)/", $data, $matched) ? $matched[0] : $default);

		case 'date':
			# perl equiv - $data =~ s/[^0-9-]//sg; return($data);
			return(preg_replace("/[^0-9-]/", '', $data));

		case 'email':
			# perl equiv - $data =~ s/[^\w\'\.\-\+@]//sg; return($data); # see http://regexlib.com/Search.aspx
			return(preg_replace("/[^\w\'\.\-\+@]/", '', $data));

		case 'username':
			# perl equiv - $data =~ s/[^\w\s,\+\-\.\_\'@]//sg; return($data);
			return(preg_replace("/[^\w\s,\+\-\.\_\'@]/", '', $data));

			# Failed test return the default.
			return($default);

		case 'filepath':
			# perl equiv - $data =~ s/[^\w\/\.\-]//sg; $data =~ s/\.{2,}//sg; return($data);
			return(pathinfo($data, PATHINFO_DIRNAME));

		case 'filename':
			# perl equiv - $data =~ s/.*[\/\\](.*)/$1/; return($data);
			# Strip everything prior to final forward / backslash
			return(pathinfo($data, PATHINFO_BASENAME));

		case 'filename_no_extension':
			# File Name with out extension
			# perl equiv - $data =~ s/\.[^.]+$//; return($data);
			return(pathinfo($data, PATHINFO_FILENAME));

		case 'file_extension':
			# perl equiv - return(lc(($data =~ m/([^.]+)$/)[0]));
			return(pathinfo($data, PATHINFO_EXTENSION));

		default:
			die('Error: Unknown CGI filter type: ' . $filter_type);
	}

	# Not Reached.
}

function _deltag($tag = '') {

	# %PRIVATE%
	# NAME:*		_deltag
	# DESCR:*		private method for use by secure_CGI_object removes unsafe HTML tags to protect against XSS attacks
	# USAGE:*		Internal Function - DO NOT USE.
	# RETURNS:*		Input stripped of some unsafe html tags.
	# %END_PRIVATE%

	# Debug
	#error_log('security.inc: _deltag() -> $tag = ' . Dumper($tag));

	html_entity_decode($tag, ENT_QUOTES);

	$tag = preg_replace('/([\x00-\x08]|[\x0b-\x0c]|[\x0e-\x1f])/', '', $tag);
	$tag = preg_replace('/<[^>]*>/', '', $tag);

	return($tag);
}