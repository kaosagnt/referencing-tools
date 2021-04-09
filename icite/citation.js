(function ($) {
	// Javascript file for iCite.
	// 
	// Author: Ian McWilliam
	// 
	// This is an adaption of Referen(C)ite from The University of Auckland NZ.
	// 
	// Licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	// International License - http://creativecommons.org/licenses/by-nc-sa/4.0/
	//
	// The Code is a complete rewrite and only the data from the original
	// Referen(C)ite is used.
	Drupal.behaviors.drupaliCite = {
		attach: function (context, settings) {

			// Select controls.
			// edit-reference-style
			// edit-publication-type .dasbdb
			// edit-reference-type .dasbdb1
			//$.debug_print(settings.ajax);

			$('#edit-reference-style').change(function() {
				$('.dasbdb').val(0);
				$('.dasbdb option:gt(0)').remove();
				$('.dasbdb1').val(0);
				$('.dasbdb1 option:gt(0)').remove();
				$('#citation-display').html('');
			});

			$('.dasbdb').change(function() {
				if ($('.dasbdb').val() == 0) {
					$('.dasbdb1').val(0);
					$('.dasbdb1 option:gt(0)').remove();
				}
				$('#citation-display').html('');
			});

		}	
	};
	
	Drupal.ajax.prototype.originalBeforeSerialize = Drupal.ajax.prototype.beforeSerialize;
	
	Drupal.ajax.prototype.beforeSerialize = function (element, options) {

		// See Drupal.ajax.prototype.beforeSerialize
		if (this.form) {
			var settings = this.settings || Drupal.settings;
			Drupal.detachBehaviors(this.form, settings, 'serialize');
		}

		//alert('here');
		
		if ($('.dasbdb').val() == 0) {
			$('.dasbdb1').val(0);
			$('.dasbdb1 option:gt(0)').remove();
		}
		
		return(this.originalBeforeSerialize(element, options));
	};
}(jQuery));