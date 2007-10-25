// vim:set noet cinoptions= sw=4 ts=4:
// This file is part of the eix project and distributed under the
// terms of the GNU General Public License v2.
//
// Copyright (c)
//   Wolfgang Frisch <xororand@users.sourceforge.net>
//   Emil Beinroth <emilbeinroth@gmx.net>
//   Martin V�th <vaeth@mathematik.uni-wuerzburg.de>

#ifndef __MASK_H__
#define __MASK_H__

#include <eixTk/exceptions.h>
#include <eixTk/ptr_list.h>
#include <portage/basicversion.h>
#include <portage/keywords.h>

#include <map>
#include <string>

class Version;
class Package;

//  >app-shells/bash-3*      <- NOT ALLOWED
//  ~app-shells/bash-3*      <- OK, BUT DOESN'T SELECT bash-3.0-r12; SELECT
//                              ONLY ~app-shells/bash-3
//  =app-shells/bash-3*      <- OK

/** A class for parsing masking definitions
 * like those in the profile and /etc/portage/package.(un)mask */
class Mask : public BasicVersion {

	public:
		/** Describes the type of a mask.
		 * Nothing specific, entry in "packages"-file but not in
		 * system-profile, entry in packages-file and in system-profile,
		 * entry in package.mask, entry in package.unmask */
		typedef enum {
			maskTypeNone, maskAllowedByProfile,
			maskInSystem,
			maskMask, maskUnmask
		} Type;

		/** Describes the comparison operator before the mask. */
		typedef enum {
			maskOpAll, maskOpEqual,
			maskOpLess, maskOpLessEqual,
			maskOpGreaterEqual, maskOpGreater,
			maskOpRevisions, maskOpGlob
		} Operator;

		typedef struct {
			const char *str;
			Mask::Operator op;
		} OperatorTable;

		static OperatorTable operators[];

	private:
		Operator m_operator; /**< Operator for mask. */
		Type m_type;   /**< Mask type for this mask. */

		std::string m_category; /**< category */
		std::string m_name;     /**< package name */
		std::string m_slot;
		bool m_test_slot;       /**< must we match a slot? */

		/** split a "mask string" into its components
		 * @param str_mask the string to be dissected
		 * @throw ExBasic on errors */
		void parseMask(const char *str) throw(ExBasic);

		/** Sets the stability & masked members of ve according to the mask
		 * @param ve Version instance to be set */
		void apply(Version *ve, Keywords::Redundant check = Keywords::RED_NOTHING);

		/** Tests if the mask applies to a Version.
		 * @param bv test this version
		 * @return true if applies. */
		bool test(const ExtendedVersion *bv) const;

	public:
		/** Parse mask-string. */
		Mask(const char *str, Type type);

		eix::ptr_list<Version> match(Package &pkg) const;

		const char *getVersion() const
		{ return m_full.c_str(); }

		const char *getName() const
		{ return m_name.c_str(); }

		const char *getCategory() const
		{ return m_category.c_str(); }

		Type get_type () const
		{ return m_type; }

		/** Sets the stability members of all version in package according to the mask.
		 * @param pkg            package you want tested
		 * @param check_name     true if name should be tested
		 * @param check_category true if category should be tested */
		void checkMask(Package& pkg, const bool check_category, const bool check_name, Keywords::Redundant check = Keywords::RED_NOTHING);

		bool ismatch(Package& pkg);
};

class KeywordMask : public Mask {
	public:

		KeywordMask(const char *str) : Mask(str, maskTypeNone)
		{ }

		std::string keywords;
		bool locally_double;
};

#endif /* __MASK_H__ */
