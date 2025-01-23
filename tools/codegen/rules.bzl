# (c) Copyright 2024 Zenseact AB
# SPDX-License-Identifier: Apache-2.0

def wheezy_template_codegen(name, data, entry_template, out = '-', extra_templates = [], **kwargs):
    native.py_binary(
        name = name,
        main = "//tools/codegen:codegen.py",
        srcs= [
            "//tools/codegen:library"
        ],
        data =
        [
            data,
            entry_template,
        ] + extra_templates,
        args = [
            "--data $(location {})".format(data),
            "--template $(location {})".format(entry_template),
            "--output {}".format(out),
        ],
        **kwargs,
    )
