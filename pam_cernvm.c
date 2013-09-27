/**
 * This PAM module is used to contextualize a CernVM using the PIN pairing 
 * mechanism.  A login name of the form #<PIN> is interpreted as 
 * contextualization request, all other login names are passed through.
 * 
 * The actual contextualization request is forwarded to 
 * /etc/cernvm/pam_action.sh, having the login name as first argument.
 *
 * This module should be high in the auth PAM stack as "sufficient" and 
 * high in the account PAM stack as "requisite" (see /etc/pam.d).
 * The shared library itself should be installed under /lib64/security/
 *
 * Mar 2013, jblomer@cern.ch
 */

#define PAM_SM_AUTH
#include <security/pam_modules.h>
#include <syslog.h>

#include <string.h>
#include <stdlib.h>

#define ACTION_FILE "/etc/cernvm/pam_action.sh"


PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{  
  const char *user;
  int retval = pam_get_user(pamh, &user, NULL);
  if (retval != PAM_SUCCESS)
    return retval;
  
  if (user && (user[0] == '#')) {
    pam_syslog(pamh, LOG_NOTICE, "running CernVM contextualization...");
    int cmd_line_length = strlen(ACTION_FILE) + 1 + strlen(user) + 1;
    char *cmd_line = (char *)malloc(cmd_line_length);
    strcpy(cmd_line, ACTION_FILE);
    strcat(cmd_line, " ");
    strcat(cmd_line, &user[1]);  // strip #
    retval = system(cmd_line);
    free(cmd_line);
    if (retval != 0) {
      pam_syslog(pamh, LOG_NOTICE, "contextualization failed (%d)", retval);
      return PAM_AUTH_ERR;
    }
    return PAM_SUCCESS;
  }
  
  return PAM_IGNORE;
}


PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return PAM_IGNORE;
}


PAM_EXTERN int
pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc ,const char **argv)
{
  const char *user;
  int retval = pam_get_user(pamh, &user, NULL);
  if (retval != PAM_SUCCESS)
    return retval;
  
  if (user && (user[0] == '#')) {
    pam_syslog(pamh, LOG_NOTICE, 
               "contextualization does not have an account, stopping login.");
    return PAM_USER_UNKNOWN;
  }
  
  return PAM_IGNORE;
}


PAM_EXTERN int
pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return PAM_IGNORE;
}


PAM_EXTERN int
pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{      
  return PAM_IGNORE;
}


PAM_EXTERN int
pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  return PAM_IGNORE;
}

