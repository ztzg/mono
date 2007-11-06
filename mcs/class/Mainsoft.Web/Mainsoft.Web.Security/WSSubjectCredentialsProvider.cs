using System;
using System.Collections.Generic;
using System.Text;
using com.ibm.websphere;
using javax.security.auth;

namespace Mainsoft.Web.Security
{
	class WSSubjectCredentialsProvider : vmw.@internal.auth.SubjectCredentialsPrvider
	{				
		protected override Subject getSubject()
		{			
			return com.ibm.websphere.security.auth.WSSubject.getCallerSubject ();			
		}
	}
}
