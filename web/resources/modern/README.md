# "Modern" visual style
Hosted at http://ricegnat.com/resources/modern

## Usage
Simply link the stylesheet in the document's `<head>` 
```html
<link rel="stylesheet" href="http://ricegnat.com/resources/modern/modern.css" />
```
and add the `modern` class to the desired root element.
```html
<body class="modern">
```
The LESS stylesheet may also be linked directly if using the client-side LESS library.
```html
<link rel="stylesheet/less" type="text/css" href="http://ricegnat.com/resources/modern/modern.less" />
<script src="//cdnjs.cloudflare.com/ajax/libs/less.js/2.7.2/less.min.js" ></script>
```
### Checkboxes and radio buttons
Checkboxes and radio buttons require the following structure.
```html
<input type="checkbox" id="check1" />
<label for="check1"><span class="check small"></span>Small checkbox</label>
```
The `<label>` element *must* be immediately after the `<input>` checkbox element. Elements with the `check` class inside the `<label>` will be rendered as the visible checkbox.

The valid size classes are `small`, `medium`, and `large`. If no size class is used, the `medium` size will be used by default.

Note that radio buttons also use the `check` class to designate the visible checkbox element.
```html
<input type="radio" id="radio1" name="radio" />
<label for="radio1"><span class="check small"></span>Small radio button</label>
```
### Mobile appearance
To ensure consistent font sizing on mobile browsers, include the following `<meta>` tag in the document's `<head>`.
```html
<meta name="viewport" content="width=device-width, initial-scale=1">
```
## Convenience classes
### Headers
Classes for header styles `h1` through `h6` are provided to apply header styling without using the header elements to help maintain semantic HTML.
```html
<div class="h1">Looks like a header</div>
```
### noselect
The `noselect` class will disable user selection on the tagged element.
### tip
The `tip` class can be used for tips or reminder text.
## Extension sheets
Link extension sheets as desired.
```html
<link rel="stylesheet" href="http://ricegnat.com/resources/modern/modern.css" />
<link rel="stylesheet" href="http://ricegnat.com/resources/modern/modern.dark.css" />
```
Note that the base `modern.css` sheet is required.
### Dark theme
Uses a light-on-dark color scheme. Apply the `dark` class on the desired root element.
```html
<body class="modern dark">
```
The `dark` styling will be contained within the tagged element, allowing it to be used in conjunction with the default scheme.
```html
<body class="modern">
    <div>
        <h1>Modern</h1>
        <h2>Visual Style</h2>
        <h3>Inspired by Windows Modern Design</h3>
        <h4>As introduced in Windows 8</h4>
        <h5 class="inline-block">Monochrome theme</h5> <span class="tip">modern.css</span>
    </div>
    <div class="dark">
        <h5 class="inline-block">Dark theme</h5> <span class="tip">modern.dark.css</span>
    </div>
</body>
```
