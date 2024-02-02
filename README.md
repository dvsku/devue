<div align="center">
    <img src="https://github.com/dvsku/devue/blob/main/.github/readme/devue.png?raw=true" width="100px"/>
    <h1>devue</h1>
    <p>
        <span>
            <img click="" src="https://img.shields.io/github/actions/workflow/status/dvsku/devue/build.yml?branch=main&style=flat-square"/>
        </span>
        <span>
            <img src="https://img.shields.io/github/downloads/dvsku/devue/total?style=flat-square"/>
        </span>
        <span>
            <img src="https://img.shields.io/github/license/dvsku/devue?style=flat-square"/>
        </span>
    </p>
    <p>
        3D model viewing software
    </p>
</div>
</br>

## About
devue is a 3D model viewing software with plugin support.</br>It was made while working on a personal project trying to reverse engineer an unknown model format and needing a way to quickly see changes.
Since then I've started learning OpenGL and expanded it further. It is meant to be extendable via model and texture plugins.

## Features
#### Natively supported models
.obj

#### Natively supported textures
.jpg .png .bmp

## Plugins
**Use 3rd party plugins at your own risk. I don't guarantee the safety of any plugin that is not included with devue release.**

devue comes with a set of included model and texture plugins that enable loading of popular formats.</br>
It can be extended by custom plugins by implementing the shared C interface.

You can use [this](https://github.com/dvsku/devue_plugin_template) template to make your own plugin. See [official plugin implementation](https://github.com/dvsku/devue_official_plugin) as an example on how to implement it.
