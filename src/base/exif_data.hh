/* Exif metadata utilities.
 * Derived from Darktable (http://www.darktable.org/)
 */

/*

    Copyright (C) 2014 Ferrero Andrea

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.


 */

/*

    These files are distributed with PhotoFlow - http://aferrero2707.github.io/PhotoFlow/

 */

#ifndef PF_EXIF_DATA_H
#define PF_EXIF_DATA_H

#include <glib.h>

#define PF_META_EXIF_NAME "custom-exif-data"

namespace PF
{

typedef struct _exif_data_t
{
	float exif_exposure;
	float exif_aperture;
	float exif_iso;
	float exif_focal_length;
	float exif_focus_distance;
	float exif_crop;
	char exif_maker[64];
	char exif_model[64];
	char exif_lens[128];
	char exif_datetime_taken[20];
} exif_data_t;

bool exif_read(exif_data_t* data, const char* path);

void exif_free (gpointer mem);

}

#endif
