<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html"/>

<xsl:template match="registry">
   <html>
   <title>PSP Registry</title>
   <body>
   System Settings
   <xsl:apply-templates/>
   </body>
   </html>
</xsl:template>

<xsl:template match="dir">
      <xsl:choose>
      <xsl:when test="(@check = 'pass')">
         <table style="background-color: white; border: 1px solid black; padding: 0.3em; margin: 0.2em;">
            <tr><td style="text-align: left;">Directory: <b><xsl:value-of select="@name"/></b></td></tr>
            <tr><th style="text-align: left;">DATA TYPE</th><th>NAME</th><th>VALUE</th></tr>
            <xsl:apply-templates/>
         </table><br/>
      </xsl:when>
      <xsl:when test="(@check = 'fail')">
         <table style="background-color: red; border: 1px solid black; padding: 0.3em; margin: 0.2em;">
            <tr><td style="text-align: left;">Directory: <b><xsl:value-of select="@name"/></b> (SHA1 checksum fails)</td></tr>
            <tr><th style="text-align: left;">DATA TYPE</th><th>NAME</th><th>VALUE</th></tr>
            <xsl:apply-templates/>
         </table><br/>
      </xsl:when>
   </xsl:choose>   
</xsl:template>

<xsl:template match="integer">
   <tr><td>Integer</td><td style="text-align: center;"><xsl:value-of select="@name"/></td><td style="text-align: right;"><xsl:value-of select="."/></td></tr>
</xsl:template>

<xsl:template match="string">
   <tr><td>String</td><td style="text-align: center;"><xsl:value-of select="@name"/></td><td style="text-align: right;"><xsl:value-of select="."/></td></tr>
</xsl:template>

<xsl:template match="secret">
   <tr><td>Secret String</td><td style="text-align: center;"><xsl:value-of select="@name"/></td><td style="text-align: right;"><xsl:value-of select="."/></td></tr>
</xsl:template>

</xsl:stylesheet>