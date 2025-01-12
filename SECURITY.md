# Security Policy

This document outlines the security risks associated with the ZMBT framework
and provides guidelines for handling and reporting security issues.

## Potential Vulnerabilities

The framework interacts with tested systems and data through serialization and foreign function interfaces.
It relies on user-provided adaptors, which may introduce vulnerabilities during test execution.
Additionally, adaptor code generated using template engines (e.g., Jinja2) can create security risks
during the rendering process if not properly managed.

## Recommendations

* Use only the framework’s built-in code generation utilities.
* Carefully review test adaptors, templates, and generated code for security issues.
* Execute code generation and testing in isolated environments.
* Apply the principle of least privilege to all generation and execution processes.
* Validate and sanitize all test inputs and template data.
* Securely manage and store sensitive data and secrets.


## Reporting

Do not report security vulnerabilities in public issues.
Instead, [open a private security advisory on GitHub](https://github.com/zenseact/zmbt-framework/security/advisories/new).

