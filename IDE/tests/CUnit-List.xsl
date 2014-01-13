<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://www.w3.org/1999/xhtml">

    <xsl:strip-space elements="*"/>
    <xsl:output indent="yes"
                method="xml"
                encoding="utf-8"
                doctype-public="-//W3C//DTD XHTML 1.1//EN"
                doctype-system="http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"/>

    <xsl:template match="CUNIT_TEST_LIST_REPORT">
        <html>
            <head>
                <meta http-equiv="Content-Type" content="application/xhtml+xml; charset=utf-8"/>
                <meta name="content-language" content="en"/>
                <link rel="shortcut icon" href="favicon.png" type="image/png"/>
                <title> Suite and Test Case Organization in Test Registry </title>
                <style type="text/css">
                    td.test_case_name {
                        text-align: left;
                        background-color: #e0e0d0;
                    }
                    td.test_active_value {
                        background-color: #e0e0d0;
                    }
                    td.test_case {
                        background-color: #e0f0d0;
                    }
                </style>
            </head>

            <body style="text-align: center">
                <xsl:apply-templates/>
            </body>
        </html>
    </xsl:template>

    <xsl:template match="CUNIT_HEADER">
        <div>
            <a href="index.html"><img src="logo.png" width="309" height="83" alt=""/></a><br/>
            <i>Moravia Microsystems, s.r.o.</i><br/><br/>
        </div>
    </xsl:template>

    <xsl:template match="CUNIT_LIST_TOTAL_SUMMARY">
        <table style="width: 50%; margin: 0 auto">
            <xsl:apply-templates/>
        </table>
    </xsl:template>

    <xsl:template match="CUNIT_LIST_TOTAL_SUMMARY_RECORD">
        <tr>
            <td style="background-color: #f0f0e0; width: 70%">
                <xsl:value-of select="CUNIT_LIST_TOTAL_SUMMARY_RECORD_TEXT" />
            </td>
            <td style="background-color: #f0e0e0">
                <xsl:value-of select="CUNIT_LIST_TOTAL_SUMMARY_RECORD_VALUE" />
            </td>
        </tr>
    </xsl:template>

    <xsl:template match="CUNIT_ALL_TEST_LISTING">
        <div>
            <h2> Listing of Registered Suites &amp; Tests </h2>
        </div>
        <table style="width: 90%; margin: 0 auto">
            <tr style="background-color: #00ccff">
                <td colspan="8"> </td>
            </tr>
            <tr>
                <td colspan="2" style="background-color: #f0f0e0; width: 44%"> </td>
                <td style="background-color: #f0f0e0; width: 14%"> <b> Initialize Function? </b> </td>
                <td style="background-color: #f0f0e0; width: 14%"> <b> Cleanup Function? </b> </td>
                <td style="background-color: #f0f0e0; width: 14%"> <b> Test Count </b> </td>
                <td style="background-color: #f0f0e0; width: 14%"> <b> Active? </b> </td>
            </tr>
            <xsl:apply-templates/>
        </table>
    </xsl:template>

    <xsl:template match="CUNIT_ALL_TEST_LISTING_SUITE">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="CUNIT_ALL_TEST_LISTING_SUITE_DEFINITION">
        <tr style="background-color: #00ccff">
            <td colspan="8"> </td>
        </tr>
        <tr>
            <td style="background-color: #f0e0f0; text-align: left"> Suite </td>
            <td style="background-color: #f0e0f0; text-align: left"> <b> <xsl:value-of select="SUITE_NAME" /> </b> </td>

            <td style="background-color: #f0e0f0"> <xsl:value-of select="INITIALIZE_VALUE" /> </td>
            <td style="background-color: #f0e0f0"> <xsl:value-of select="CLEANUP_VALUE" /> </td>
            <td style="background-color: #f0e0f0"> <xsl:value-of select="TEST_COUNT_VALUE" /> </td>
            <td style="background-color: #f0e0f0"> <xsl:value-of select="ACTIVE_VALUE" /> </td>
        </tr>
        <tr style="background-color: #00ccff">
            <td colspan="8"> </td>
        </tr>
    </xsl:template>

    <xsl:template match="CUNIT_ALL_TEST_LISTING_SUITE_TESTS">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="TEST_CASE_DEFINITION">
        <tr>
            <td class="test_case"> Test </td>
            <td colspan="4" class="test_case_name">
                <xsl:value-of select="TEST_CASE_NAME" />
            </td>
            <td class="test_active_value">
                <xsl:value-of select="TEST_ACTIVE_VALUE" />
            </td>
        </tr>
    </xsl:template>

    <xsl:template match="CUNIT_ALL_TEST_LISTING_GROUP">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="CUNIT_ALL_TEST_LISTING_GROUP_DEFINITION">
        <tr>
            <td style="background-color: #f0e0f0; width: 10%"> Suite </td>
            <td style="background-color: #e0f0f0; width: 20%" >
                <b> <xsl:value-of select="GROUP_NAME" /> </b>
            </td>

            <td style="background-color: #f0e0f0; width: 15%"> Initialize Function? </td>
            <td style="background-color: #e0f0f0; width: 5%">
                <xsl:value-of select="INITIALIZE_VALUE" />
            </td>

            <td style="background-color: #f0e0f0; width: 15%"> Cleanup Function? </td>
            <td style="background-color: #e0f0f0; width: 5%">
                <xsl:value-of select="CLEANUP_VALUE" />
            </td>

            <td style="background-color: #f0e0f0; width: 10%"> Test Count </td>
            <td style="background-color: #e0f0f0; width: 5%">
                <xsl:value-of select="TEST_COUNT_VALUE" />
            </td>
        </tr>
    </xsl:template>

    <xsl:template match="CUNIT_ALL_TEST_LISTING_GROUP_TESTS">
        <tr>
            <td style="background-color: #e0f0d0"> Test Cases </td>
            <td colspan="7" style="text-align: left; background-color: #e0e0d0">
                <xsl:for-each select="TEST_CASE_NAME">
                    <xsl:apply-templates/> <br />
                </xsl:for-each>
            </td>
        </tr>
        <tr style="background-color: #00ccff">
            <td colspan="8"> </td>
        </tr>
    </xsl:template>

    <xsl:template match="CUNIT_FOOTER">
        <div>
            <br/><br/>
            <hr style="width: 90%; margin: 0 auto"/><br/>
            <xsl:apply-templates/>
        </div>
    </xsl:template>

</xsl:stylesheet>
