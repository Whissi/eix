// vim:set noet cinoptions= sw=4 ts=4:
// This file is part of the eix project and distributed under the
// terms of the GNU General Public License v2.
//
// Copyright (c)
//   Wolfgang Frisch <xororand@users.sourceforge.net>
//   Emil Beinroth <emilbeinroth@gmx.net>
//   Martin V�th <vaeth@mathematik.uni-wuerzburg.de>

#ifndef __CASCADINGPROFILE_H__
#define __CASCADINGPROFILE_H__

#include <eixTk/exceptions.h>
#include <portage/mask_list.h>
#include <portage/package.h>

#include <map>
#include <string>
#include <vector>
#include <cstdlib>

class PortageSettings;
class Package;

/** Access to the cascading profile pointed to by /etc/make.profile. */
class CascadingProfile {

	public:

	protected:
		std::vector<std::string>      m_profile_files; /**< List of files in profile. */
		PortageSettings *m_portagesettings; /**< Profilesettings to which this instance "belongs" */

		MaskList<Mask> m_system;         /**< Packages in m_system profile. */
		MaskList<Mask> m_system_allowed; /**< Packages that are not in m_system profile but only allowed to have specific versions.*/
		MaskList<Mask> m_package_masks;  /**< Masks from package.masks */

	private:

		/** Cycle through profile.
		 * Look for parent profile of the profile pointed to by path_buffer Write the path for the new
		 * profile into path_buffer and return true Return false if there is no parent profile.
		 * @param path_buffer Path to the profile (new profile is written into this thing)
		 * @return false if there is no parent profile */
		bool getParentProfile(std::string &path_buffer);

		/** Read all "packages" files found in profile.
		 * Populate m_system and m_system_allowed.
		 * @return true if data was changed */
		bool readPackages(const std::string &line);
		bool readPackageMasks(const std::string &line);

		void ReadLink(std::string &path) const;
	public:
		CascadingProfile(PortageSettings *portagesettings)
		{ m_portagesettings = portagesettings; }

		/** Read all "make.defaults" files previously added by listadd... */
		void readMakeDefaults();

		/** Read all mask/system files previously added by listadd...
		 * and clear this list of files afterwards.
		 * @return true if at least one file changed data. */
		bool readremoveFiles();

		/** Cycle through profile and put path to files into
		 * m_profile_files. */
		void listaddProfile(const char *profile_dir = NULL) throw(ExBasic);

		/** Put file into m_profile_files */
		void listaddFile(const char *file)
		{ m_profile_files.push_back(file); }

		/** Clear m_profile_files */
		void listclear()
		{ m_profile_files.clear(); }

		void applyMasks(Package *p) const;

		/** Get all m_system packages. */
		const MaskList<Mask> *getSystemPackages() const {
			return &m_system;
		}

		/** Get packages that are not in m_system profile but only allowed to have specific versions .*/
		const MaskList<Mask> *getAllowedPackages() const {
			return &m_system_allowed;
		}

		const MaskList<Mask> *getPackageMasks() const {
			return &(m_package_masks);
		}
};


#endif /* __CASCADINGPROFILE_H__ */
