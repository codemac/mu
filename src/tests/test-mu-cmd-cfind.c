/* -*- mode: c; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
** 
** Copyright (C) 2008-2011 Dirk-Jan C. Binnema <djcb@djcbsoftware.nl>
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 3, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software Foundation,
** Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  
**  
*/

#if HAVE_CONFIG_H
#include "config.h"
#endif /*HAVE_CONFIG_H*/

#include <glib.h>
#include <glib/gstdio.h>

#include "../mu-query.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "test-mu-common.h"
#include "src/mu-store.h"

static gchar*
fill_contacts_cache (void)
{
	gchar *cmdline, *tmpdir;
	
	tmpdir = test_mu_common_get_random_tmpdir();
	cmdline = g_strdup_printf ("%s index --muhome=%s --maildir=%s"
				   " --quiet",
				   MU_PROGRAM, tmpdir, MU_TESTMAILDIR);
	
	g_assert (g_spawn_command_line_sync (cmdline, NULL, NULL,
					     NULL, NULL));
	g_free (cmdline);
		
	return tmpdir;
}


static void
test_mu_cfind_plain (void)
{
	gchar *muhome, *cmdline, *output, *erroutput;
	
	muhome = fill_contacts_cache ();
	g_assert (muhome != NULL);

	cmdline = g_strdup_printf ("%s cfind --muhome=%s --format=plain "
				   "'testmu\\.xxx?'",
				   MU_PROGRAM, muhome);
	
	output = erroutput = NULL;
	g_assert (g_spawn_command_line_sync (cmdline, &output, &erroutput,
					     NULL, NULL));
	g_assert_cmpstr (output,
			 ==,
			 "Helmut Kröger hk@testmu.xxx\n"
			 "Mü testmu@testmu.xx\n");
	g_free (cmdline);
	g_free (muhome);
	g_free (output);
	g_free (erroutput);
}

static void
test_mu_cfind_bbdb (void)
{
	gchar *muhome, *cmdline, *output, *erroutput;
	
	muhome = fill_contacts_cache ();
	g_assert (muhome != NULL);

	cmdline = g_strdup_printf ("%s cfind --muhome=%s --format=bbdb "
				   "'testmu\\.xxx?'",
				   MU_PROGRAM, muhome);
	
	output = erroutput = NULL;
	g_assert (g_spawn_command_line_sync (cmdline, &output, &erroutput,
					     NULL, NULL));
	g_assert_cmpstr (output,
			 ==,
			 ";; -*-coding: utf-8-emacs;-*-\n"
			 ";;; file-version: 6\n"
			 
			 "[\"Helmut\" \"Kröger\" nil nil nil nil (\"hk@testmu.xxx\") "
			 "((creation-date . \"2011-05-19\") "
			 "(time-stamp . \"1970-01-01\")) nil]\n"

			 "[\"Mü\" \"\" nil nil nil nil (\"testmu@testmu.xx\") "
			 "((creation-date . \"2011-05-19\") "
			 "(time-stamp . \"1970-01-01\")) nil]\n");

	g_free (cmdline);
	g_free (muhome);
	g_free (output);
	g_free (erroutput);
}


static void
test_mu_cfind_wl (void)
{
	gchar *muhome, *cmdline, *output, *erroutput;
	
	muhome = fill_contacts_cache ();
	g_assert (muhome != NULL);

	cmdline = g_strdup_printf ("%s cfind --muhome=%s --format=wl "
				   "'testmu\\.xxx?'",
				   MU_PROGRAM, muhome);
	
	output = erroutput = NULL;
	g_assert (g_spawn_command_line_sync (cmdline, &output, &erroutput,
					     NULL, NULL));
	g_assert_cmpstr (output,
			 ==,
			 "hk@testmu.xxx \"HelmutK\" \"Helmut Kröger\"\n"
			 "testmu@testmu.xx \"Mü\" \"Mü\"\n");
	g_free (cmdline);
	g_free (muhome);
	g_free (output);
	g_free (erroutput);
}


static void
test_mu_cfind_mutt_alias (void)
{
	gchar *muhome, *cmdline, *output, *erroutput;
	
	muhome = fill_contacts_cache ();
	g_assert (muhome != NULL);

	cmdline = g_strdup_printf ("%s cfind --muhome=%s --format=mutt-alias "
				   "'testmu\\.xxx?'",
				   MU_PROGRAM, muhome);
	
	output = erroutput = NULL;
	g_assert (g_spawn_command_line_sync (cmdline, &output, &erroutput,
					     NULL, NULL));
	g_assert_cmpstr (output,
			 ==,
			 "alias HelmutK Helmut Kröger <hk@testmu.xxx>\n"
			 "alias Mü Mü <testmu@testmu.xx>\n");
	g_free (cmdline);
	g_free (muhome);
	g_free (output);
	g_free (erroutput);
}

static void
test_mu_cfind_mutt_ab (void)
{
	gchar *muhome, *cmdline, *output, *erroutput;
	
	muhome = fill_contacts_cache ();
	g_assert (muhome != NULL);

	cmdline = g_strdup_printf ("%s cfind --muhome=%s --format=mutt-ab "
				   "'testmu\\.xxx?'",
				   MU_PROGRAM, muhome);
	
	output = erroutput = NULL;
	g_assert (g_spawn_command_line_sync (cmdline, &output, &erroutput,
					     NULL, NULL));
	g_assert_cmpstr (output,
			 ==,
			 "hk@testmu.xxx\tHelmut Kröger\t\n"	
			 "testmu@testmu.xx\tMü\t\n");
	g_free (cmdline);
	g_free (muhome);
	g_free (output);
	g_free (erroutput);
}


static void
test_mu_cfind_org_contact (void)
{
	gchar *muhome, *cmdline, *output, *erroutput;
	
	muhome = fill_contacts_cache ();
	g_assert (muhome != NULL);

	cmdline = g_strdup_printf ("%s cfind --muhome=%s --format=org-contact "
				   "'testmu\\.xxx?'",
				   MU_PROGRAM, muhome);
	
	output = erroutput = NULL;
	g_assert (g_spawn_command_line_sync (cmdline, &output, &erroutput,
					     NULL, NULL));
	g_assert_cmpstr (output,
			 ==,
			 "* Helmut Kröger\n"
			 ":PROPERTIES:\n"
			 ":EMAIL: hk@testmu.xxx\n"
			 ":END:\n\n"

			 "* Mü\n"
			 ":PROPERTIES:\n"
			 ":EMAIL: testmu@testmu.xx\n"
			 ":END:\n\n");
	g_free (cmdline);
	g_free (muhome);
	g_free (output);
	g_free (erroutput);
}



static void
test_mu_cfind_csv (void)
{
	gchar *muhome, *cmdline, *output, *erroutput;
	
	muhome = fill_contacts_cache ();
	g_assert (muhome != NULL);

	cmdline = g_strdup_printf ("%s cfind --muhome=%s --format=csv "
				   "'testmu\\.xxx?'",
				   MU_PROGRAM, muhome);
	
	output = erroutput = NULL;
	g_assert (g_spawn_command_line_sync (cmdline, &output, &erroutput,
					     NULL, NULL));
	g_assert_cmpstr (output,
			 ==,
			 "Helmut Kröger,hk@testmu.xxx\n"
			 "Mü,testmu@testmu.xx\n");
	g_free (cmdline);
	g_free (muhome);
	g_free (output);
	g_free (erroutput);
}




int
main (int argc, char *argv[])
{
	int rv;
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/mu-cmd-cfind/test-mu-cfind-plain", test_mu_cfind_plain);
	g_test_add_func ("/mu-cmd-cfind/test-mu-cfind-bbdb",  test_mu_cfind_bbdb);
	g_test_add_func ("/mu-cmd-cfind/test-mu-cfind-wl",  test_mu_cfind_wl);
	g_test_add_func ("/mu-cmd-cfind/test-mu-cfind-mutt-alias",
			 test_mu_cfind_mutt_alias);
	g_test_add_func ("/mu-cmd-cfind/test-mu-cfind-mutt-ab",
			 test_mu_cfind_mutt_ab);
	g_test_add_func ("/mu-cmd-cfind/test-mu-cfind-org-contact",
			 test_mu_cfind_org_contact);
	g_test_add_func ("/mu-cmd-cfind/test-mu-cfind-csv",
			 test_mu_cfind_csv);
	
	g_log_set_handler (NULL,
			   G_LOG_LEVEL_MASK | G_LOG_LEVEL_WARNING|
			   G_LOG_FLAG_FATAL| G_LOG_FLAG_RECURSION,
			   (GLogFunc)black_hole, NULL);

	rv = g_test_run ();

	return rv;
}
