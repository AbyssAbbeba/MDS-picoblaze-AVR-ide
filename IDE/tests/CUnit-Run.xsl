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

    <xsl:template match="CUNIT_TEST_RUN_REPORT">
        <html>
            <head>
                <meta http-equiv="Content-Type" content="application/xhtml+xml; charset=utf-8"/>
                <meta name="content-language" content="en"/>
                <link rel="shortcut icon" href="favicon.png" type="image/png"/>
                <title> Automated Test Run Summary Report </title>
                <style type="text/css">
                    td.passed {
                        background-color: #50ff50;
                        text-align: left;
                    }
                    tr.testsuccess {
                        background-color: #e0f0d0;
                    }
                    td.failed {
                        background-color: #ff5050;
                        text-align: left;
                    }
                    td.testfailure {
                        background-color: #ff9090;
                    }
                    td.filename {
                        width: 50%;
                        text-align: left;
                        background-color: #e0eee0;
                    }
                    td.linenumber {
                        width: 10%;
                        text-align: left;
                        background-color: #e0eee0;
                    }
                    td.condition {
                        width: 85%;
                        text-align: left;
                        background-color: #e0eee0;
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

    <xsl:template match="CUNIT_RESULT_LISTING">
        <div>
            <h2> Automated Test Run Results </h2>
        </div>
        <table style="width: 90%; margin: 0 auto">
            <tr>
                <td style="width: 25%"> </td>
                <td style="width: 25%"> </td>
                <td style="width: 25%"> </td>
                <td style="width: 25%"> </td>
            </tr>
            <xsl:apply-templates/>
        </table>
    </xsl:template>

    <xsl:template match="CUNIT_RUN_SUITE">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="SUITE_NAME">
    </xsl:template>

    <xsl:template match="CUNIT_RUN_SUITE_SUCCESS">
        <tr style="background-color: #f0e0f0">
            <td colspan="4" style="text-align: left">
                Running Suite <xsl:value-of select="SUITE_NAME"/>
            </td>
        </tr>
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="CUNIT_RUN_GROUP">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="CUNIT_RUN_GROUP_SUCCESS">
        <tr style="background-color: #f0e0f0">
            <td colspan="4" style="text-align: left">
                Running Group <xsl:apply-templates/>
            </td>
        </tr>
    </xsl:template>

    <xsl:template match="CUNIT_RUN_TEST_RECORD">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="CUNIT_RUN_TEST_SUCCESS">
        <tr class="testsuccess">
            <td> </td>
            <td colspan="2" style="text-align: left">
                Running test <xsl:apply-templates/>...
            </td>
            <td class="passed"> Passed </td>
        </tr>
    </xsl:template>
    <xsl:template match="CUNIT_RUN_TEST_FAILURE">
        <tr style="background-color: #e0f0d0">
            <td> </td>
            <td colspan="2" style="text-align: left">
                Running test <xsl:value-of select="TEST_NAME"/>...
            </td>
            <td class="failed"> Failed </td>
        </tr>

        <tr>
            <td colspan="4" class="testfailure">
                <table style="width: 100%">
                    <tr>
                        <th style="width: 15%"> File Name </th>
                        <td class="filename">
                            <xsl:value-of select="FILE_NAME"/>
                        </td>
                        <th style="width: 20%"> Line Number </th>
                        <td class="linenumber">
                            <xsl:value-of select="LINE_NUMBER"/>
                        </td>
                    </tr>
                    <tr>
                        <th style="width: 15%"> Condition </th>
                        <td colspan="3" class="condition">
                            <xsl:value-of select="CONDITION"/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </xsl:template>
    <xsl:template match="CUNIT_RUN_SUITE_FAILURE">
        <tr>
            <td colspan="3" style="background-color: #f0b0f0; text-align: left">
                Running Suite <xsl:value-of select="SUITE_NAME"/>...
            </td>
            <td style="background-color: #ff7070">
                <xsl:value-of select="FAILURE_REASON"/>
            </td>
        </tr>
    </xsl:template>

    <xsl:template match="CUNIT_RUN_GROUP_FAILURE">
        <tr>
            <td colspan="3" style="background-color: #f0b0f0; text-align: left">
                Running Group <xsl:value-of select="GROUP_NAME"/>...
            </td>
            <td style="background-color: #ff7070">
                <xsl:value-of select="FAILURE_REASON"/>
            </td>
        </tr>
    </xsl:template>

    <xsl:template match="CUNIT_RUN_SUMMARY">
        <table style="width: 90%; margin: 0 auto; margin-top: 20px">
            <tr style="background-color: skyblue">
                <th colspan="6"> Cumulative Summary for Run </th>
            </tr>
            <tr>
                <th style="width: 15%; background-color: #ffffc0"> Type </th>
                <th style="width: 17%; background-color: #ffffc0"> Total </th>
                <th style="width: 17%; background-color: #ffffc0"> Run </th>
                <th style="width: 17%; background-color: #ffffc0"> Succeeded </th>
                <th style="width: 17%; background-color: #ffffc0"> Failed </th>
                <th style="width: 17%; background-color: #ffffc0"> Inactive </th>
            </tr>
            <xsl:for-each select="CUNIT_RUN_SUMMARY_RECORD">
                <tr style="background-color: lightgreen">
                    <td> <xsl:value-of select="TYPE" /> </td>
                    <td> <xsl:value-of select="TOTAL" /> </td>
                    <td> <xsl:value-of select="RUN" /> </td>
                    <td> <xsl:value-of select="SUCCEEDED" /> </td>
                    <td> <xsl:value-of select="FAILED" /> </td>
                    <td> <xsl:value-of select="INACTIVE" /> </td>
                </tr>
            </xsl:for-each>
        </table>
    </xsl:template>

    <xsl:template match="CUNIT_FOOTER">
        <div>
            <br/>
            <hr style="width: 90%; margin: 0 auto"/><br/>
            <xsl:apply-templates/>
        </div>
    </xsl:template>

</xsl:stylesheet>
