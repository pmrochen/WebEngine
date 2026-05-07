/*
 *	Name: SeekOrigin
 *	Author: Pawel Mrochen
 */

#pragma once

namespace filesystem {

enum class SeekOrigin
{
	BEGIN = 0,		// = SEEK_SET,
	CURRENT = 1,	// = SEEK_CUR,
	END	= 2			// = SEEK_END
};

} // namespace filesystem
