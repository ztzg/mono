﻿<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="_Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>OutputCacheProvider Test 1</title>
</head>
<body>
    <form id="form1" runat="server">
    <div><%= AppDomain.CurrentDomain.GetData ("BEGIN_CODE_MARKER") %><pre runat="server" id="output"></pre><%= AppDomain.CurrentDomain.GetData ("END_CODE_MARKER") %></div>
    </form>
</body>
</html>
