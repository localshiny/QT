---
sort: 2
---

# Install LocalShiny R package

LocalShiny is an R package used to deploy Shiny web apps to hosted service, the LocalShiny web. Its source codes are available on [GitHub]( https://github.com/).

## 2.1 Supported os

Windows, Linux and Mac OS are currently supported.

## 2.2 Dependencies

| Name | Source |
| -------------------------------------------------------- | -------------------------------------------- |
| [renv](https://rstudio.github.io/renv/)                  | https://rstudio.github.io/renv/              |
| [RCrul](https://CRAN.R-project.org/package=RCurl)        | https://CRAN.R-project.org/package=RCurl     |
| [jsonlite](https://CRAN.R-project.org/package=jsonlites) | https://CRAN.R-project.org/package=jsonlites |
| [zip](https://CRAN.R-project.org/package=zip)            | https://CRAN.R-project.org/package=zip       |
| [xml2](https://CRAN.R-project.org/package=xml2)          | https://CRAN.R-project.org/package=xml2      |

## 2.3 Installation

You can install the released version of LocalShiny from GitHub with:

```r
#install package dependencies
install.packages(c("renv", "RCurl", "jsonlite", "zip", "xml2"))
#install.packages("devtools")
devtools::install_github("localshiny/localshiny")
```

After the package has been installed, load it into your R session.

```r
library(localshiny)
```
