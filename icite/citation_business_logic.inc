<?php

/**
 * @file
 * iCite Business logic file for Drupal module icite.
 *
 * Author: Ian McWilliam
 *
 * This is an adaption of Referen(C)ite from The University of Auckland NZ.
 *
 * Licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0
 * International License - http://creativecommons.org/licenses/by-nc-sa/4.0/
 *
 * The Code is a complete rewrite and only the data from the original
 * Referen(C)ite is used.
 */

global $reference_style;
global $reference_publication_limits;
global $citation_publication_type;
global $citation_reference_type;
global $citation_directory_names;
global $citation_base_URL;

$citation_base_URL = '/referencing-citation/iCite';

$citation_publication_type_text = array(
	'1' => 'Books',
	'2' => 'Print articles',
	'3' => 'Electronic articles',
	'4' => 'Webpage',
	'5' => 'Conference papers',
	'6' => 'Report',
	'7' => 'Other',
	'8' => 'Articles'
);

$citation_reference_type_text = array(
	# Books
	'1' => 'Book, no author',
	'2' => 'Book, one author',
	'3' => 'Book, two authors',
	'4' => 'Book, three to five authors',
	'5' => 'Book, six or more authors',
	'6' => 'Book, editor as \'author\'',
	'7' => 'Book, chapters by different authors',
	'8' => 'Book, multiple books by same author',
	'9' => 'Book, similar information from multiple works',
	'10' => 'Book, one source quoted in another',
	'11' => 'Book, translated',
	'12' => 'Book, editions',
	'13' => 'Ebook',
	'14' => 'Book, three authors',
	'15' => 'Book, four or more authors',
	'16' => 'Book, editor and author',
	'17' => 'Book, second edition',
	'18' => 'Book, volume in a multivolume work',
	'19' => 'Book, The Bible',
	'20' => 'Book, the Qur\'an',
	'21' => 'Book, classical author',
	'22' => 'Book, edited book',
	'23' => 'Book, chapter in edited book',
	'24' => 'Book, two or three authors',
	'25' => 'Book, four to six authors',
	'26' => 'Book, seven or more authors',
	'27' => 'Book, chapter or article in a book',
	'28' => 'Book, chapter or article in an edited book',
	'29' => 'Book, two to six authors',
	'30' => 'Book, chapter or section in book',
	'31' => 'Book, corporate author',
	'32' => 'Book, three or more authors',
	'33' => 'Book, no author or editor',
	'34' => 'Book, three to twenty authors',
	'35' => 'Book, twenty one or more authors',
	'36' => 'Book, Group author',

	# Journals / Mags / Newspapers
	'40' => 'Journal article, no author',
	'41' => 'Journal article, one author',
	'42' => 'Journal article, two authors',
	'43' => 'Journal article, three to five authors',
	'44' => 'Journal article, six or more authors',
	'45' => 'Journal article, multiple articles by same author',
	'46' => 'Journal article, similar information from multiple works',
	'47' => 'Journal article, one source quoted in another',
	'48' => 'Magazine article',
	'49' => 'Newspaper article, no author',
	'50' => 'Newspaper article, one author',
	'51' => 'Journal article, three or more authors',
	'52' => 'Journal article, three authors',
	'53' => 'Journal article, four or more authors',
	'54' => 'Newspaper article from database',
	'55' => 'Journal article, three to twenty authors',
	'56' => 'Journal article, twenty one or more authors',

	# Electronic Articles / Web
	'60' => 'Library database article / internet journal article',
	'61' => 'Library database article',
	'62' => 'Internet journal article',
	'63' => 'Webpage on news website',

	'70' => 'Webpage created by organisation',
	'71' => 'Webpage, known author',
	'72' => 'Webpage, no author',
	'73' => 'Webpage, no publication date',
	'74' => 'Webpage, quotation',
	'75' => 'Social media',
	'76' => 'Webpage / document on the internet',
	'77' => 'Webpage, two or more authors',
	'78' => 'Document on the internet, two or more authors',

	# Conference
	'80' => 'Conference paper, published',
	'81' => 'Conference paper, online',
	'82' => 'Conference paper, unpublished',
	'83' => 'Conference presentation',

	# Report
	'90' => 'Report by an organisation',
	'91' => 'Government report',
	'92' => 'University report',
	'93' => 'Report, online',
	'94' => 'Government report, print',
	'95' => 'Government report, online',
	'96' => 'Report',

	# Other
	'100' => 'Thesis or dissertation',
	'101' => 'Film or video recording',
	'102' => 'Image from the web',
	'103' => 'Patent',
	'104' => 'Radio or TV broadcast',
	'105' => 'Statistics and numeric data',
	'106' => 'Artwork',
	'107' => 'Lecture (unpublished) / personal communication',
	'108' => 'Lecture or lecture notes online',
	'109' => 'Lecture or lecture notes, published',
	'110' => 'Tutorial / lecture handout',
	'111' => 'Acts of Parliament (includes bills)',
	'112' => 'Australian Bureau of Statistics (AusStats)',
	'113' => 'Australian Bureau of Statistics (Census information)',
	'114' => 'Brochure',
	'115' => 'Legal authorities (cases)',
	'116' => 'Microfiche / microfilm document',
	'117' => 'Podcast (from the internet)',
	'118' => 'Standard',
	'119' => 'Study guide',
	'120' => 'Video or audio (from the internet)',
	'121' => 'Australian Bureau of Statistics',
	# 122 free to reuse
	'123' => 'Image from Book (or other media)',
	'124' => 'Image from the internet (no author)',
	'125' => 'Video from the internet',
	'126' => 'Interview (unpublished) / personal communication',
	'127' => 'Online lecture / Lecture notes',
	'128' => 'Episode in radio, podcast or TV series',
	'129' => 'Television broadcast or Episode in a Series'
);

# 1. Citation Type Select
$reference_style = array(
	'0' => '- - -',
	'1' => 'APA 7',
	'2' => 'MLA 8',
	'3' => 'Chicago 17',
	'4' => 'Harvard WesternSydU',
	'5' => 'Vancouver'
);

$citation_directory_names = array(
	'1' => 'apa7',
	'2' => 'mla8',
	'3' => 'chicago17',
	'4' => 'uws_harvard',
	'5' => 'vancouver'
);

$reference_publication_limits = array(
	'0' => 1,
	'1' => 7,
	'2' => 6,
	'3' => 6,
	'4'	=> 7,
	'5' => 7
);

# 2. Publication Type Select
$citation_publication_type = array(
	'0' => array(
		'0' => '- - -'
	),
	'1' => array(						# Citation Type // APA
		'0' => '- - -',
		'1' => $citation_publication_type_text['1'],
		'2' => $citation_publication_type_text['8'],
		'4' => $citation_publication_type_text['4'],
		'5' => $citation_publication_type_text['5'],
		'6' => $citation_publication_type_text['6'],
		'7' => $citation_publication_type_text['7']
	),
	'2' => array(						# Citation Type // mla
		'0' => '- - -',
		'1' => $citation_publication_type_text['1'],
		'2' => $citation_publication_type_text['2'],
		'3' => $citation_publication_type_text['3'],
		'4' => $citation_publication_type_text['4'],
		'5' => $citation_publication_type_text['5'],
		'6' => $citation_publication_type_text['7']
	),
	'3' => array(						# Citation Type // chicago
		'0' => '- - -',
		'1' => $citation_publication_type_text['1'],
		'2' => $citation_publication_type_text['2'],
		'3' => $citation_publication_type_text['3'],
		'4' => $citation_publication_type_text['4'],
		'5' => $citation_publication_type_text['5'],
		'6' => $citation_publication_type_text['7']
	),
	'4' => array(						# Citation Type
		'0' => '- - -',
		'1' => $citation_publication_type_text['1'],
		'2' => $citation_publication_type_text['2'],
		'3' => $citation_publication_type_text['3'],
		'4' => $citation_publication_type_text['4'],
		'5' => $citation_publication_type_text['5'],
		'6' => $citation_publication_type_text['6'],
		'7' => $citation_publication_type_text['7']
	),
	'5' => array(						# Citation Type
		'0' => '- - -',
		'1' => $citation_publication_type_text['1'],
		'2' => $citation_publication_type_text['2'],
		'3' => $citation_publication_type_text['3'],
		'4' => $citation_publication_type_text['4'],
		'5' => $citation_publication_type_text['5'],
		'6' => $citation_publication_type_text['6'],
		'7' => $citation_publication_type_text['7']
	)
);

# 3. Reference Details
$citation_reference_type = array(
	'0' => array(
		'0' => array(
			'0' => '- - -',
		)
	),
	'1' => array(						# Citation Type
		'0' => array(					# Publication Type
			'0' => '- - -'
		),
		'1' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['1'],
			'2' => $citation_reference_type_text['2'],
			'3' => $citation_reference_type_text['3'],
			'4' => $citation_reference_type_text['34'],
			'5' => $citation_reference_type_text['35'],
			'14' => $citation_reference_type_text['36'],
			'6' => $citation_reference_type_text['6'],
			'7' => $citation_reference_type_text['7'],
			'8' => $citation_reference_type_text['8'],
			'9' => $citation_reference_type_text['9'],
			'10' => $citation_reference_type_text['10'],
			'11' => $citation_reference_type_text['11'],
			'12' => $citation_reference_type_text['12'],
			'13' => $citation_reference_type_text['13']
		),
		'2' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['40'],
			'2' => $citation_reference_type_text['41'],
			'3' => $citation_reference_type_text['42'],
			'4' => $citation_reference_type_text['55'],
			'5' => $citation_reference_type_text['56'],
			'6' => $citation_reference_type_text['45'],
			'7' => $citation_reference_type_text['46'],
			'8' => $citation_reference_type_text['47'],
			'9' => $citation_reference_type_text['48'],
			'10' => $citation_reference_type_text['49'],
			'11' => $citation_reference_type_text['50'],
			'12' => $citation_reference_type_text['61'],
			'13' => $citation_reference_type_text['62']
		),
		'4' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['70'],
			'2' => $citation_reference_type_text['71'],
			'3' => $citation_reference_type_text['72'],
			'4' => $citation_reference_type_text['73'],
			'5' => $citation_reference_type_text['63'],
			'6' => $citation_reference_type_text['74'],
			'7' => $citation_reference_type_text['75']
		),
		'5' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['80']
		),
		'6' => array(					# Publication Type
			'0' => '- - -',
			'5' => $citation_reference_type_text['91'],
			'3' => $citation_reference_type_text['92'],
			'4' => $citation_reference_type_text['96']

		),
		'7' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['107'],
			'2' => $citation_reference_type_text['109'],
			'9' => $citation_reference_type_text['108'],
			'3' => $citation_reference_type_text['100'],
			'4' => $citation_reference_type_text['101'],
			'5' => $citation_reference_type_text['102'],
			'6' => $citation_reference_type_text['103'],
			'7' => $citation_reference_type_text['129'],
			'8' => $citation_reference_type_text['121'],
			'10' => $citation_reference_type_text['117']

		)
	),
	'2' => array(						# Citation Type // mla
		'0' => array(					# Publication Type
			'0' => '- - -'
		),
		'1' => array(					# Publication Type // books
			'0' => '- - -',
			'1' => $citation_reference_type_text['33'],
			'2' => $citation_reference_type_text['2'],
			'3' => $citation_reference_type_text['3'],
			'4' => $citation_reference_type_text['32'],
			'5' => $citation_reference_type_text['6'],
			'6' => $citation_reference_type_text['7'],
			'7' => $citation_reference_type_text['8'],
			'8' => $citation_reference_type_text['9'],
			'9' => $citation_reference_type_text['10'],
			'10' => $citation_reference_type_text['11'],
			'11' => $citation_reference_type_text['30'],
			'12' => $citation_reference_type_text['31'],
			'13' => $citation_reference_type_text['13']
		),
		'2' => array(					# Publication Type // print
			'0' => '- - -',
			'1' => $citation_reference_type_text['40'],
			'2' => $citation_reference_type_text['41'],
			'3' => $citation_reference_type_text['42'],
			'4' => $citation_reference_type_text['51'],
			'5' => $citation_reference_type_text['45'],
			'6' => $citation_reference_type_text['46'],
			'7' => $citation_reference_type_text['47'],
			'8' => $citation_reference_type_text['48'],
			'9' => $citation_reference_type_text['49'],
			'10' => $citation_reference_type_text['50']
		),
		'3' => array(					# Publication Type // electronic articles
			'0' => '- - -',
			'1' => $citation_reference_type_text['60']
		),
		'4' => array(					# Publication Type // web pages
			'0' => '- - -',
			'1' => $citation_reference_type_text['71'],
			'2' => $citation_reference_type_text['72']
		),
		'5' => array(					# Publication Type // conference papers
			'0' => '- - -',
			'1' => $citation_reference_type_text['80'],
			'2' => $citation_reference_type_text['83']
		),
		'6' => array(					# Publication Type // others
			'0' => '- - -',
			'1' => $citation_reference_type_text['107'],
			'2' => $citation_reference_type_text['108'],
			'3' => $citation_reference_type_text['100'],
			'4' => $citation_reference_type_text['101'],
			'5' => $citation_reference_type_text['128'],
			'6' => $citation_reference_type_text['106'],
			'7' => $citation_reference_type_text['121'],
			'8' => $citation_reference_type_text['95'],
			'9' => $citation_reference_type_text['123'],
			'10' => $citation_reference_type_text['124'],
			'11' => $citation_reference_type_text['117'],
			'12' => $citation_reference_type_text['114'],
			'13' => $citation_reference_type_text['125']
		)
	),
	'3' => array(						# Citation Type // chicago
		'0' => array(					# Publication Type
			'0' => '- - -'
		),
		'1' => array(					# Publication Type // books
			'0' => '- - -',
			'1' => $citation_reference_type_text['1'],
			'2' => $citation_reference_type_text['2'],
			'3' => $citation_reference_type_text['3'],
			'4' => $citation_reference_type_text['14'],
			'5' => $citation_reference_type_text['15'],
			'6' => $citation_reference_type_text['6'],
			'7' => $citation_reference_type_text['16'],
			'8' => $citation_reference_type_text['7'],
			'9' => $citation_reference_type_text['8'],
			'10' => $citation_reference_type_text['9'],
			'11' => $citation_reference_type_text['10'],
			'12' => $citation_reference_type_text['11'],
			'13' => $citation_reference_type_text['17'],
			'14' => $citation_reference_type_text['18'],
			'15' => $citation_reference_type_text['19'],
			'16' => $citation_reference_type_text['20'],
			'17' => $citation_reference_type_text['21']
		),
		'2' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['40'],
			'2' => $citation_reference_type_text['41'],
			'3' => $citation_reference_type_text['42'],
			'4' => $citation_reference_type_text['52'],
			'5' => $citation_reference_type_text['53'],
			'6' => $citation_reference_type_text['45'],
			'7' => $citation_reference_type_text['46'],
			'8' => $citation_reference_type_text['47'],
			'9' => $citation_reference_type_text['48'],
			'10' => $citation_reference_type_text['49'],
			'11' => $citation_reference_type_text['50']
		),
		'3' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['61'],
			'2' => $citation_reference_type_text['62']
		),
		'4' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['71'],
			'2' => $citation_reference_type_text['72']
		),
		'5' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['80'],
			'2' => $citation_reference_type_text['81'],
			'3' => $citation_reference_type_text['82']
		),
		'6' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['126'],
			'2' => $citation_reference_type_text['127'],
			'3' => $citation_reference_type_text['100'],
			'4' => $citation_reference_type_text['101']
		)
	),
	'4' => array(						# Citation Type
		'0' => array(					# Publication Type
			'0' => '- - -'
		),
		'1' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['1'],
			'2' => $citation_reference_type_text['2'],
			'3' => $citation_reference_type_text['24'],
			'4' => $citation_reference_type_text['25'],
			'5' => $citation_reference_type_text['26'],
			'6' => $citation_reference_type_text['6'],
			'7' => $citation_reference_type_text['27'],
			'8' => $citation_reference_type_text['28'],
			'9' => $citation_reference_type_text['8'],
			'10' => $citation_reference_type_text['10'],
			'11' => $citation_reference_type_text['13']
		),
		'2' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['41'],
			'2' => $citation_reference_type_text['50']
		),
		'3' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['61'],
			'2' => $citation_reference_type_text['62'],
			'3' => $citation_reference_type_text['54']
		),
		'4' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['76']
		),
		'5' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['80']
		),
		'6' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['94'],
			'2' => $citation_reference_type_text['95']
		),
		'7' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['108'],
			'2' => $citation_reference_type_text['107'],
			'3' => $citation_reference_type_text['100'],
			'4' => $citation_reference_type_text['101'],
			'5' => $citation_reference_type_text['102'],
			'6' => $citation_reference_type_text['103'],
			'7' => $citation_reference_type_text['104'],
			'8' => $citation_reference_type_text['110'],
			'9' => $citation_reference_type_text['111'],
			'10' => $citation_reference_type_text['112'],
			'11' => $citation_reference_type_text['113'],
			'12' => $citation_reference_type_text['114'],
			'13' => $citation_reference_type_text['115'],
			'14' => $citation_reference_type_text['116'],
			'15' => $citation_reference_type_text['117'],
			'16' => $citation_reference_type_text['118'],
			'17' => $citation_reference_type_text['119'],
			'18' => $citation_reference_type_text['120']
		)
	),
	'5' => array(						# Citation Type
		'0' => array(					# Publication Type
			'0' => '- - -'
		),
		'1' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['1'],
			'2' => $citation_reference_type_text['2'],
			'3' => $citation_reference_type_text['3'],
			'4' => $citation_reference_type_text['32'],
			'5' => $citation_reference_type_text['6'],
			'6' => $citation_reference_type_text['27'],
			'7' => $citation_reference_type_text['28']
		),
		'2' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['41'],
			'2' => $citation_reference_type_text['50']
		),
		'3' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['61'],
			'2' => $citation_reference_type_text['62'],
			'3' => $citation_reference_type_text['54']
		),
		'4' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['72'],
			'2' => $citation_reference_type_text['78']
		),
		'5' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['80']
		),
		'6' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['95']
		),
		'7' => array(					# Publication Type
			'0' => '- - -',
			'1' => $citation_reference_type_text['107'],
			'2' => $citation_reference_type_text['108'],
			'3' => $citation_reference_type_text['100']
		)
	)
);
?>