# LiteRGSS namespace
# 
# It contains every LiteRGSS classes and modules 
module LiteRGSS
  # Error triggered by various functions for some reasons
  Error = StandardError.new
  # LiteRGSS configs
  # Holds every constants that help LiteRGSS to generate a good graphic window
  module Config
    # The witdh of the screen (between 160 and the maximum fullscreen width)
    ScreenWidth = 640
    # The height of the screen (between 144 and the maximum fullscreen height)
    ScreenHeight = 480
    # Scale of the Window display (factor to ScreenWidth and ScreenHeight)
    ScreenScale = 1
    # The title of the window
    Title = "LiteRGSS"
    # The number of frames per second (Only used by Input)
    FrameRate = 60
    # If the game is in fullscreen
    FullScreen = false
    # If the Texture should be smoothed or not
    SmoothScreen = false
  end
  # Graphics module manages the frame and window display
  module Graphics
    # Error triggered when you use a Graphic function without starting the window display ( `Graphics.start` )
    StoppedError = StandardError.new
    # Error triggered when the user closes the Window
    ClosedWindowError = StandardError.new
    module_function
    # Start the Graphic display : show a window according to the LiteRGSS::Config constants and unlock the Graphics functions
    def start
    end
    # Stop the Grahics display : closes the window and lock the Graphics functions
    def stop
    end
    # Show the current frame to the screen and catch every SFML Events that LiteRGSS reads (Input)
    def update
    end
    # Take a snapshot of the frame before the last Graphics.update
    # @return [Bitmap]
    def snap_to_bitmap
    end
    # Freeze the Graphics by showing the current frame on top of each sprite (triggers a Graphics.update to take the current frame)
    def freeze
    end
    # Show a transition between the frozen Graphics and the new Graphic state
    # @param nb_frame [Integer] the number of frames the transition lasts
    # @param bitmap [LiteRGSS::Bitmap] the bitmap to use to perform the transition
    def transition(nb_frame = 8, bitmap = nil)
    end
    # Return an array containing the possible 32 bits fullscreen resolutions
    # @return [Array<Array(width, height)>]
    def list_resolutions
    end
    # Reload the sprite stack (z_order processing)
    # @return [self]
    def reload_stack
    end
    # Update only the sprite display (does not increase the frame_count as well)
    # @return [self]
    def update_no_input
    end
    # Update only the Inputs (no graphic processing but perform message processing)
    # @return [self]
    def update_only_input
    end
    # Resize the screen (inGameScreen)
    # @param width [Integer] new width of the window (will affect LiteRGSS::Config::ScreenWidth)
    # @param height [Integer] new height of the window (will affect LiteRGSS::Config::ScreenHeight)
    # @note If you change LiteRGSS::Config::ScreenScale, the actual window size will be (width * ScreenScale) x (height * ScreenScale)
    # @return self
    def resize_screen(width, height)
    end
    class << self
      # Return the number of frames shown on the screen
      # @return [Integer]
      attr_accessor :frame_count
      # Return the game screen width
      # @return [Integer]
      attr_reader :width
      # Return the game screen height
      # @return [Integer]
      attr_reader :height
      # Return the screen brightness (between 0 and 255)
      # @return [Integer]
      attr_accessor :brightness
      # @return [LiteRGSS::Shader, nil] shader used on the whole screen
      attr_accessor :shader
    end
  end
  # Class that defines a rectangular surface of a Graphical element
  class Rect
    # x position of the surface
    # @return [Integer]
    attr_accessor :x
    # y position of the surface
    # @return [Integer]
    attr_accessor :y
    # width of the surface
    # @return [Integer]
    attr_accessor :width
    # height of the surface
    # @return [Integer]
    attr_accessor :height
    # Create a new surface
    # @param x [Integer] x position of the surface
    # @param y [Integer] y position of the surface
    # @param width [Integer] width of the surface
    # @param height [Integer] height of the surface
    # @overload new(width, height)
    # @overload new(x, width, height)
    # @overload new(x, y, width, height)
    def initialize(x, y, width, height)
    end
    # Set the parameters of the surface
    # @param x [Integer, nil] x position of the surface
    # @param y [Integer, nil] y position of the surface
    # @param width [Integer, nil] width of the surface
    # @param height [Integer, nil] height of the surface
    # @return [self]
    def set(x, y = nil, width = nil, height = nil)
    end
    # Test if the Rect is equal to an other Rect
    # @param rect [Rect] the other rect
    # @return [Boolean]
    def eql?(rect)
    end
    alias == eql?
    alias === eql?
    # Convert the rect to a string that can be shown to the user
    # @return [String] (x, y, width, height)
    def to_s
    end
    alias inspect to_s
    # Set all the rect coordinates to 0
    # @return [self]
    def empty
    end
    # Serialize the rect
    # @param limit [Object] ignored
    # @return [String] serialization of the rect
    def _dump(limit)
    end
    # Load a rect from a serialized rect
    # @param str [String] the serialized rect
    # @return [Rect]
    def self._load(str)
    end
  end
  # Class that describes RGBA colors
  class Color
    # The red value of the color
    # @return [Integer]
    attr_accessor :red
    # The green value of the color
    # @return [Integer]
    attr_accessor :green
    # The blue value of the color
    # @return [Integer]
    attr_accessor :blue
    # The alpha opacity of the color
    # @return [Integer]
    attr_accessor :alpha
    # Create a new color
    # @param red [Integer, nil] between 0 and 255
    # @param green [Integer, nil] between 0 and 255
    # @param blue [Integer, nil] between 0 and 255
    # @param alpha [Integer, nil]  between 0 and 255 (default : 255)
    def initialize(red, green, blue, alpha = 255)
    end
    # Set the color parameters
    # @param red [Integer, nil] between 0 and 255
    # @param green [Integer, nil] between 0 and 255
    # @param blue [Integer, nil] between 0 and 255
    # @param alpha [Integer, nil]  between 0 and 255
    # @return [self]
    def set(red, green = nil, blue = nil, alpha = nil)
    end
    # Test if the Color is equal to an other Color
    # @param color [Color] the other color
    # @return [Boolean]
    def eql?(color)
    end
    alias == eql?
    alias === eql?
    # Convert the color to a string that can be shown to the user
    # @return [String] (r, g, b, a)
    def to_s
    end
    alias inspect to_s
    # Serialize the color
    # @param limit [Object] ignored
    # @return [String]
    def _dump(limit)
    end
    # Load a color from a serialized color
    # @param str [String] the serialized color
    # @return [Color]
    def self._load(str)
    end
  end
  # Class that describe tones (added/modified colors to the surface)
  class Tone
    # The red value of the tone
    # @return [Integer]
    attr_accessor :red
    # The green value of the tone
    # @return [Integer]
    attr_accessor :green
    # The blue value of the tone
    # @return [Integer]
    attr_accessor :blue
    # The gray modifier of the tone (255 => grayscale)
    # @return [Integer]
    attr_accessor :gray
    # Create a new tone
    # @param red [Integer, nil] between -255 and 255
    # @param green [Integer, nil] between -255 and 255
    # @param blue [Integer, nil] between -255 and 255
    # @param gray [Integer, nil]  between 0 and 255 (default : 255)
    def initialize(red, green, blue, gray = 255)
    end
    # Set the tone parameters
    # @param red [Integer, nil] between -255 and 255
    # @param green [Integer, nil] between -255 and 255
    # @param blue [Integer, nil] between -255 and 255
    # @param gray [Integer, nil]  between 0 and 255
    # @return [self]
    def set(red, green = nil, blue = nil, gray = nil)
    end
    # Test if the Tone is equal to an other Tone
    # @param tone [Tone] the other tone
    # @return [Boolean]
    def eql?(tone)
    end
    alias == eql?
    alias === eql?
    # Convert the tone to a string that can be shown to the user
    # @return [String] (r, g, b, a)
    def to_s
    end
    alias inspect to_s
    # Serialize the tone
    # @param limit [Object] ignored
    # @return [String]
    def _dump(limit)
    end
    # Load a tone from a serialized tone
    # @param str [String] the serialized tone
    # @return [Tone]
    def self._load(str)
    end
  end
  # Class of all the element that can be disposed
  class Disposable
    # Dispose the element (and free its memory)
    # @return [self]
    def dispose
    end
    # Tell if the element was disposed
    # @return [Boolean]
    def disposed?
    end
  end
  # Class of all the element that can be drawn in a Viewport or the Graphic display
  class Drawable < Disposable
  end
  # Class that stores an image loaded from file or memory
  class Bitmap < Disposable
    # Create a new bitmap
    # @param filename_or_memory [String]
    # @param from_memory [Boolean]
    def initialize(filename_or_memory, from_memory = false)
    end
    # Returns the width of the Bitmap
    # @return [Integer]
    def width
    end
    # Returns the heigth of the Bitmap
    # @return [Integer]
    def height
    end
    # Returns the surface of the Bitmap
    # @return [Rect]
    def rect
    end
    # Update the content of the bitmap (when operations where done on the bitmap)
    def update
    end
    # Blit an other bitmap to this bitmap (replace pixels!)
    # @param x [Integer] dest x coordinate
    # @param y [Integer] dest y coordinate
    # @param source_bitmap [Bitmap] bitmap containing the copied pixels
    # @param source_rect [Rect] surface of the source_bitmap containing the copied pixels
    def blt(x, y, source_bitmap, source_rect)
    end
    # Clear a portion of the bitmap
    # @param x [Integer] left corner coordinate
    # @param y [Integer] top corner coordinate
    # @param width [Integer] width of the cleared surface
    # @param height [Integer] height of the cleared surface
    def clear_rect(x, y, width, height)
    end
    # Fill a portion of the bitmap with a color
    # @param x [Integer] left corner coordinate
    # @param y [Integer] top corner coordinate
    # @param width [Integer] width of the filled surface
    # @param height [Integer] height of the filled surface
    # @param color [Color] color to fill
    def fill_rect(x, y, width, height, color)
    end
    # Convert bitmap to PNG
    # @return [String, nil] contents of the PNG, nil if couldn't be converted to PNG
    def to_png
    end
    # Save the bitmap to a PNG file
    # @param filename [String] Name of the PNG file
    # @return [Boolean] success of the operation
    def to_png_file(filename)
    end
  end
  # Class that is dedicated to perform Image operation in Memory before displaying those operations inside a Bitmap
  class Image < Disposable
    # Create a new image
    # @param filename_or_memory [String]
    # @param from_memory [Boolean]
    def initialize(filename_or_memory, from_memory = false)
    end
    # Returns the width of the image
    # @return [Integer]
    def width
    end
    # Returns the heigth of the image
    # @return [Integer]
    def height
    end
    # Returns the surface of the image
    # @return [Rect]
    def rect
    end
    # Copy the image content to the bitmap (Bitmap must be the same size of the image)
    # @param bitmap [Bitmap]
    # @return [self]
    def copy_to_bitmap(bitmap)
    end
    # Blit an other image to this image (process alpha)
    # @param x [Integer] dest x coordinate
    # @param y [Integer] dest y coordinate
    # @param source [Image] image containing the copied pixels
    # @param source_rect [Rect] surface of the source containing the copied pixels
    def blt(x, y, source, source_rect)
    end
    # Blit an other image to this image (replace the pixels)
    # @param x [Integer] dest x coordinate
    # @param y [Integer] dest y coordinate
    # @param source [Image] image containing the copied pixels
    # @param source_rect [Rect] surface of the source containing the copied pixels
    def blt!(x, y, source, source_rect)
    end
    # Stretch blit an other image to this image (process alpha)
    # @param dest_rect [Rect] surface of the current image where to copy pixels
    # @param source [Image] image containing the copied pixels
    # @param source_rect [Rect] surface of the source containing the copied pixels
    def stretch_blt(dest_rect, source, source_rect)
    end
    # Stretch blit an other image to this image (replace the pixels)
    # @param dest_rect [Rect] surface of the current image where to copy pixels
    # @param source [Image] image containing the copied pixels
    # @param source_rect [Rect] surface of the source containing the copied pixels
    def stretch_blt!(dest_rect, source, source_rect)
    end
    # Clear a portion of the image
    # @param x [Integer] left corner coordinate
    # @param y [Integer] top corner coordinate
    # @param width [Integer] width of the cleared surface
    # @param height [Integer] height of the cleared surface
    def clear_rect(x, y, width, height)
    end
    # Fill a portion of the image with a color
    # @param x [Integer] left corner coordinate
    # @param y [Integer] top corner coordinate
    # @param width [Integer] width of the filled surface
    # @param height [Integer] height of the filled surface
    # @param color [Color] color to fill
    def fill_rect(x, y, width, height, color)
    end
    # Get a pixel color
    # @param x [Integer] x coordinate of the pixel
    # @param y [Integer] y coordinate of the pixel
    # @return [Color, nil] nil means x,y is outside of the Image surface
    def get_pixel(x, y)
    end
    # Get a pixel alpha
    # @param x [Integer] x coordinate of the pixel
    # @param y [Integer] y coordinate of the pixel
    # @return [Integer, 0]
    def get_pixel_alpha(x, y)
    end
    # Set a pixel color
    # @param x [Integer] x coordinate of the pixel
    # @param y [Integer] y coordinate of the pixel
    # @param color [Color] new color of the pixel
    # @return [self]
    def set_pixel(x, y, color)
    end
    # Change the alpha of all the pixel that match the input color
    # @param color [Color] color to match
    # @param alpha [Integer] new apha value of the pixel that match color
    def create_mask(color, alpha)
    end
    # Convert Image to PNG
    # @return [String, nil] contents of the PNG, nil if couldn't be converted to PNG
    def to_png
    end
    # Save the Image to a PNG file
    # @param filename [String] Name of the PNG file
    # @return [Boolean] success of the operation
    def to_png_file(filename)
    end
  end
  # Class that describes a surface of the screen where texts and sprites are shown (with some global effect)
  class Viewport < Disposable
    # The surface of the viewport on the screen
    # @return [Rect]
    attr_accessor :rect
    # The Tone effect of the viewport
    # @return [Tone]
    attr_accessor :tone
    # The aditive Color effect of the viewport
    # @return [Color]
    attr_accessor :color
    # The offset x of the viewport's contents
    # @return [Integer]
    attr_accessor :ox
    # The offset y of the viewport's contents
    # @return [Integer]
    attr_accessor :oy
    # Viewport content visibility
    # @return [Boolean]
    attr_accessor :visible
    # The viewport z property
    # @return [Numeric]
    attr_accessor :z
    # Angle of the viewport contents
    # @return [Integer]
    attr_accessor :angle
    # Zoom of the viewport contents
    # @return [Integer]
    attr_accessor :zoom
    # @return [Shader, BlendMode] Blend Mode of the viewport (can be specified in the shader)
    attr_accessor :blendmode
    # @return [Shader, BlendMode] Shader of the viewport (include the BlendMode properties)
    attr_accessor :shader
    # Create a new Viewport
    # @param x [Integer] x position of the surface
    # @param y [Integer] y position of the surface
    # @param width [Integer] width of the surface
    # @param height [Integer] height of the surface
    # @overload new(width, height)
    # @overload new(x, width, height)
    # @overload new(x, y, width, height)
    def initialize(x, y, width, height)
    end
    # Reload the viewport sprite stack
    # Used for Z processing
    # @return [self]
    def reload_stack
    end
    # Return the viewport "index" (used to know if the viewport has been created after an other sprite or viewport when z are the same
    # @return [Integer]
    def __index__
    end
    # Does nothing (RGSS compatibility)
    def update
    end
  end
  # Class that describe a sprite shown on the screen or inside a viewport
  # @note Sprites cannot be saved, loaded from file nor cloned in the memory
  class Sprite < Drawable
    # Create a new Sprite
    # @param viewport [Viewport, Window, nil] the viewport in which the sprite is shown, can be a Window
    def initialize(viewport = nil)
    end
    # Define the position of the sprite
    # @param x [Numeric]
    # @param y [Numeric]
    # @return [self]
    def set_position(x, y)
    end
    # Define the origine of the sprite (inside the bitmap)
    # @param ox [Numeric]
    # @param oy [Numeric]
    # @return [self]
    def set_origin(ox, oy)
    end
    # Define the zoom of the sprite when shown on screen
    # @param zoom [Numeric] the zoom factor
    def zoom=(zoom)
    end
    # Return the sprite index to know if it has been created before an other sprite (in the same viewport)
    # @return [Integer]
    def __index__
    end
    # Bitmap shown by the sprite
    # @return [Bitmap, nil]
    attr_accessor :bitmap
    # Surface of the sprite's bitmap show on the screen
    # @return [Rect]
    attr_accessor :src_rect
    # If the sprite is shown or not
    # @return [Boolean]
    attr_accessor :visible
    # X coordinate of the sprite
    # @return [Numeric]
    attr_accessor :x
    # Y coordinate of the sprite
    # @return [Numeric]
    attr_accessor :y
    # The z Coordinate of the sprite (unused by LiteRGSS)
    # @return [Numeric]
    attr_accessor :z
    # The origin x of the bitmap's pixels
    # @return [Numeric]
    attr_accessor :ox
    # The origin y of the bitmap's pixels
    # @return [Numeric]
    attr_accessor :oy
    # The rotation of the sprite in degree
    # @return [Numeric]
    attr_accessor :angle
    # The zoom scale in width of the sprite
    # @return [Numeric]
    attr_accessor :zoom_x
    # The zoom scale in height of the sprite
    # @return [Numeric]
    attr_accessor :zoom_y
    # The opacity of the sprite
    # @return [Numeric]
    attr_accessor :opacity
    # The sprite viewport
    # @return [LiteRGSS::Viewport, LiteRGSS::Window, nil]
    attr_reader :viewport
    # If the sprite texture is mirrored
    # @return [Boolean]
    attr_accessor :mirror
    # Return the sprite width
    # @return [Integer]
    attr_reader :width
    # Return the sprite height
    # @return [Integer]
    attr_reader :height
  end
  # Class that describes a text shown on the screen or inside a viewport
  # @note Text cannot be saved, loaded from file nor cloned in the memory
  class Text < Drawable
    # Create a new Text
    # @param font_id [Integer] the id of the font to use to draw the text (loads the size and default colors from that)
    # @param viewport [Viewport, Window, nil] the viewport in which the text is shown, can be a Window
    # @param x [Integer] the x coordinate of the text surface
    # @param y [Integer] the y coordinate of the text surface
    # @param width [Integer] the width of the text surface
    # @param height [Integer] the height of the text surface
    # @param str [String] the text shown by this object
    # @param align [0, 1, 2] the align of the text in its surface (best effort => no resize), 0 = left, 1 = center, 2 = right
    # @param outlinesize [Integer, nil] the size of the text outline
    # @param color_id [Integer, nil] ID of the color took from Fonts
    # @param size_id [Integer, nil] ID of the size took from Fonts
    def initialize(font_id, viewport, x, y, width, height, str, align = 0, outlinesize = nil, color_id = nil, size_id = nil)
    end
    # Define the position of the text
    # @param x [Numeric]
    # @param y [Numeric]
    # @return [self]
    def set_position(x, y)
    end
    # The x coordinate of the text surface
    # @return [Numeric]
    attr_accessor :x
    # The y coordinate of the text surface
    # @return [Numeric]
    attr_accessor :y
    # The width of the text surface
    # @return [Numeric]
    attr_accessor :width
    # The height of the text surface
    # @return [Numeric]
    attr_accessor :height
    # The size of the text outline
    # @return [Integer]
    attr_accessor :outline_thickness
    # The font size of the text
    # @return [Integer]
    attr_accessor :size
    # Set the font size of the text
    # @param size [Integer]
    # @return [self]
    def set_size(size)
    end
    # The alignment of the text (0 = left, 1 = center, 2 = right)
    # @return [Integer]
    attr_accessor :align
    # The color of the text
    # @return [Color]
    attr_accessor :fill_color
    # The color of the outline
    # @return [Color]
    attr_accessor :outline_color
    # Load a color from a font_id
    # @param font_id [Integer] id of the font where to load the colors
    # @return self
    def load_color(font_id)
    end
    # Text shown by this Object
    # @return [String]
    attr_accessor :text
    # If the Text is visible
    # @return [Boolean]
    attr_accessor :visible
    # If the text is drawn as in Pokemon DPP / RSE / HGSS / BW
    # @return [Boolean]
    attr_accessor :draw_shadow
    # The number of character the object should draw
    # @return [Integer]
    attr_accessor :nchar_draw
    # Return the real width of the text
    # @return [Integer]
    attr_accessor :real_width
    # Opacity of the text
    # @return [Integer]
    attr_accessor :opacity
    # Return the width of the given string if drawn by this Text object
    # @param text [String]
    # @return [Integer]
    def text_width(text)
    end
    # The Text z property
    # @return [Numeric]
    attr_accessor :z
    # Return the text index to know if it has been created before an other sprite/text/viewport in the same viewport
    # @return [Integer]
    def __index__
    end
    # Return the Text viewport
    # @return [LiteRGSS::Viewport, LiteRGSS::Window, nil]
    attr_reader :viewport
    # If the text should be shown in italic
    # @return [Boolean]
    attr_accessor :italic
    # If the text should be shown in bold
    # @return [Boolean]
    attr_accessor :bold
  end
  # Class used to show a Window object on screen.
  #
  # A Window is an object that has a frame (built from #window_builder and #windowskin) and some contents that can be Sprites or Texts.
  class Window < Drawable
    # Create a new Window
    # @param viewport [Viewport, nil]
    def initialize(viewport)
    end
    # Update the iner Window Animation (pause sprite & cursor sprite)
    # @return [self]
    def update
    end
    # Lock the window vertice calculation (background)
    # @return [self]
    def lock
    end
    # Unlock the window vertice calculation and force the calculation at the same time (background)
    # @return [self]
    def unlock
    end
    # Tell if the window vertice caculation is locked or not
    # @return [Boolean]
    def locked?
    end
    # @return [Viewport, nil] viewport in which the Window is shown
    attr_reader :viewport
    # @return [Bitmap] Windowskin used to draw the Window frame
    attr_accessor :windowskin
    # @return [Integer] Width of the Window
    attr_accessor :width
    # @return [Integer] Height of the Window
    attr_accessor :height
    # Change the size of the window
    # @param width [Integer] new width
    # @param height [Integer] new height
    # @return [self]
    def set_size(width, height)
    end
    # @return [Array(Integer, Integer, Integer, Integer, Interger, Integer)] the window builder of the Window
    # @note Array contain the 6 following values : [middle_tile_x, middle_tile_y, middle_tile_width, middle_tile_height, contents_border_x, contents_border_y]
    #       The frame is calculated from the 4 first value, the 2 last values gives the offset in x/y between the border of the frame and the border of the contents.
    attr_accessor :window_builder
    # @return [Integer] X position of the Window
    attr_accessor :x
    # @return [Integer] Y position of the Window
    attr_accessor :y
    # Change the position of the window on screen
    # @param x [Integer] new x position
    # @param y [Integer] new y position
    # @return [self]
    def set_position(x, y)
    end
    # @return [Integer] z order position of the Window in the Viewport/Graphics
    attr_accessor :z
    # @return [Integer] origin x of the contents of the Window in the Window View
    attr_accessor :ox
    # @return [Integer] origin y of the contents of the Window in the Window View
    attr_accessor :oy
    # Change the contents origin x/y in the Window View
    # @param ox [Integer]
    # @param oy [Integer]
    # @return [self]
    def set_origin(ox, oy)
    end
    # @return [Rect] cursor rect giving the coordinate of the cursor and the size of the cursor (to perform zoom operations)
    attr_accessor :cursor_rect
    # @return [Bitmap, nil] cursor bitmap used to show the cursor when the Window is active
    attr_accessor :cursorskin
    # @return [Bitmap, nil] Bitmap used to show the pause animation (there's 4 cells organized in a 2x2 matrix to show the pause animation)
    attr_accessor :pauseskin
    # @return [Boolean] if the pause animation is shown (message)
    attr_accessor :pause
    # @return [Integer, nil] x coordinate of the pause sprite in the Window (if nil, middle of the window)
    attr_accessor :pause_x
    # @return [Integer, nil] y coordinate of the pause sprite in the Window (if nil, bottom of the window)
    attr_accessor :pause_y
    # @return [Boolean] if the Window show the cursor
    attr_accessor :active
    # @return [Boolean] if the Window draw the frame by stretching the border (true) or by repeating the middle border tiles (false)
    attr_accessor :stretch
    # @return [Integer] opacity of the whole Window
    attr_accessor :opacity
    # @return [Integer] opacity of the Window frame
    attr_accessor :back_opacity
    # @return [Integer] opacity of the Window contents (sprites/texts)
    # @note It erase the opacity attribute of the texts/sprites
    attr_accessor :contents_opacity
    # @return [Rect] rect corresponding to the view of the Window (Viewport compatibility)
    attr_reader :rect
    # @return [Boolean] if the window is visible or not
    attr_accessor :visible
    # @return [Integer] internal index of the Window in the Viewport stack when it was created
    attr_reader :__index__
  end
  # Class allowing to draw Shapes in a viewport
  class Shape < Drawable
    # Constant telling the shape to draw a circle
    CIRCLE = :circle
    # Constant telling the shape to draw a convex shape
    CONVEX = :convex
    # Constant telling the shape to draw a rectangle
    RECTANGLE = :rectangle
    # @overload initialize(viewport, type, radius, num_pts)
    #   Create a new Circle shape
    #   @param viewport [Viewport] viewport in which the shape is shown
    #   @param type [Symbol] must be :circle
    #   @param radius [Numeric] radius of the circle (note : the circle is show from it's top left box corner and not its center)
    #   @param num_pts [Integer] number of points to use in order to draw the circle shape
    # @overload initialize(viewport, type, num_pts = 4)
    #   Create a new Convex shape
    #   @param viewport [Viewport] viewport in which the shape is shown
    #   @param type [Symbol] must be :convex
    #   @param num_pts [Integer] number of points to use in order to draw the convex shape
    # @overload initialize(viewport, type, width, height)
    #   Create a new Rectangle shape
    #   @param viewport [Viewport] viewport in which the shape is shown
    #   @param type [Symbol] must be :rectangle
    #   @param width [Integer] width of the rectangle
    #   @param height [Integer] height of the rectangle
    def initialize(*args)
    end
    # @return [Bitmap, nil] Bitmap used to make a specific drawing inside the shape (bitmap is show inside the border of the shape)
    attr_accessor :bitmap
    # @return [Rect] source rect used to tell which part of the bitmap is shown in the shape
    attr_accessor :src_rect
    # @return [Integer] x coordinate of the shape in the viewport
    attr_accessor :x
    # @return [Integer] y coordinate of the shape in the viewport
    attr_accessor :y
    # Set the new coordinate of the shape in the viewport
    # @param x [Integer]
    # @param y [Integer]
    # @return [self]
    def set_position(x, y)
    end
    # @return [Integer] z order of the Shape in the viewport
    attr_accessor :z
    # @return [Integer] origin x of the Shape
    attr_accessor :ox
    # @return [Integer] origin y of the Shape
    attr_accessor :oy
    # Change the origin of the Shape
    # @param ox [Integer]
    # @param oy [Integer]
    # @return [self]
    def set_origin(ox, oy)
    end
    # @return [Numeric] angle of the shape
    attr_accessor :angle
    # @return [Numeric] zoom_x of the shape
    attr_accessor :zoom_x
    # @return [Numeric] zoom_y of the shape
    attr_accessor :zoom_x
    # @return [Numeric] zoom of the shape (x&y at the same time)
    attr_writer :zoom
    # @return [Viewport] viewport in which the Shape is shown
    attr_reader :viewport
    # @return [Boolean] if the shape is visible
    attr_accessor :visible
    # @return [Numeric] number of point to build the shape (can be modified only with circle and convex)
    attr_accessor :point_count
    # Retreive the coordinate of a point
    # @param index [Integer] index of the point in the point list
    # @return [Array(Integer, Integer)]
    def get_point(index)
    end
    # Update the coordinate of a point of a Convex shape (does nothing for rectangle Shape and Circle Shape)
    # @param index [Integer] index of the point in the point list
    # @param x [Numeric] x coordinate of the point
    # @param y [Numeric] y coordinate of the point
    # @return [self]
    def set_point(index, x, y)
    end
    # @return [Color] color of the shape (or multiplied to the bitmap)
    attr_accessor :color
    # @return [Color] outline color of the shape
    attr_accessor :outline_color
    # @return [Numeric] size of the outline of the shape
    attr_accessor :outline_thickness
    # @return [Integer] internal index of the shape in the viewport when it was created
    attr_reader :__index__
    # @return [Numeric] radius of a circle shape (-1 if not a circle shape)
    attr_accessor :radius
    # @return [Symbol] type of the shape (:circle, :convex or :rectangle)
    attr_reader :type
    # @return [Numeric] width of the shape (updatable only of :rectangle)
    attr_accessor :width
    # @return [Numeric] height of the shape (updatable only for :rectangle)
    attr_accessor :height
    # @return [Shader, nil] shader used to draw the shape
    attr_accessor :shader
    # @return [BlendMode, nil] blend mode used to draw the shape
    attr_accessor :blendmode
  end
  # Class that allow to draw tiles on a row
  class SpriteMap < Drawable
    # Create a new SpriteMap
    # @param viewport [Viewport] viewport used to draw the row
    # @param tile_width [Integer] width of a tile
    # @param tile_count [Integer] number of tile to draw in the row
    def initialize(viewport, tile_width, tile_count)
    end
    # Set the position of the SpriteMap
    # @param x [Numeric]
    # @param y [Numeric]
    # @return [self]
    def set_position(x, y)
    end
    # Set the origin of the textures of the SpriteMap
    # @param ox [Numeric]
    # @param oy [Numeric]
    # @return [self]
    def set_origin(ox, oy)
    end
    # Clear the SpriteMap
    def reset
    end
    # Set the tile to draw at a certain position in the row
    # @param index [Integer] Index of the tile in the row
    # @param bitmap [Bitmap] Bitmap to use in order to draw the tile
    # @param rect [Rect] surface of the bitmap to draw in the tile
    def set(index, bitmap, rect)
    end
    # @return [Viewport] viewport used to draw the row
    attr_reader :viewport
    # @return [Numeric] X position
    attr_accessor :x
    # @return [Numeric] Y position
    attr_accessor :y
    # @return [Integer] Z index
    attr_accessor :z
    # @return [Numeric] origin X
    attr_accessor :ox
    # @return [Numeric] origin Y
    attr_accessor :oy
    # @return [Numeric] scale of each tiles in the SpriteMap
    attr_accessor :tile_scale
  end
  # Module that holds information about text fonts.
  #
  # You can define fonts loaded from a ttf file, you have to associate a default size, fill color and outline color to the font
  # 
  # You can define outline color and fill_color without defining a font but do not create a text with a font_id using the id of these color, it could raise an error, use load_color instead.
  module Fonts
    module_function
    # Load a ttf
    # @param font_id [Integer] the ID of the font you want to use to recall it in Text
    # @param filename [String] the filename of the ttf file.
    # @return [self]
    def load_font(font_id, filename)
    end
    # Define the default size of a font
    # @param font_id [Integer] the ID of the font
    # @param size [Integer] the default size
    # @return [self]
    def set_default_size(font_id, size)
    end
    # Define the fill color of a font
    # @param font_id [Integer] the ID of the font
    # @param color [Color] the fill color
    # @return [self]
    def define_fill_color(font_id, color)
    end
    # Define the outline color of a font
    # @param font_id [Integer] the ID of the font
    # @param color [Color] the outline color
    # @return [self]
    def define_outline_color(font_id, color)
    end
    # Define the shadow color of a font (WIP)
    # @param font_id [Integer] the ID of the font
    # @param color [Color] the shadow color
    # @return [self]
    def define_shadow_color(font_id, color)
    end
    # Retreive the default size of a font
    # @param font_id [Integer] the ID of the font
    # @return [Integer]
    def get_default_size(font_id)
    end
    # Retreive the fill color of a font
    # @param font_id [Integer] the ID of the font
    # @return [Color]
    def get_fill_color(font_id)
    end
    # Retreive the outline color of a font
    # @param font_id [Integer] the ID of the font
    # @return [Color]
    def get_outline_color(font_id)
    end
    # Retreive the shadow color of a font
    # @param font_id [Integer] the ID of the font
    # @return [Color]
    def get_shadow_color(font_id)
    end
  end
  # Module that helps to know the input state during the game time
  # 
  # The virtual keys of the Input module are :
  # :A, :B, :X, :Y, :L, :R, :L2, :R2, :L3, :R3, :START (:start), :SELECT (:select), :HOME (:home), :UP (:up), :DOWN (:down), :LEFT (:left), :RIGHT (:right)
  #
  # The binding of these keys can be changed by accessing `Input::Keys`, the id of the keys are one of the constants of `Input::Keyboard` or (-32 * joy_id - button_id - 1) for joypads.
  module Input
    module_function
    # Tell if a key is pressed
    # @param sym [Symbol] symbol of the key
    # @return [Boolean]
    def press?(sym)
    end
    # Tell if a key has been triggered
    # @param sym [Symbol] symbol of the key
    # @return [Boolean]
    def trigger?(sym)
    end
    # Tell if a key has been triggered and is still pressed (each period of time ~8 frames after the 20 first frames)
    # @param sym [Symbol] symbol of the key
    # @return [Boolean]
    def repeat?(sym)
    end
    # Tell if a key has been released
    # @param sym [Symbol] symbol of the key
    # @return [Boolean]
    def released?(sym)
    end
    # Indicate the direction of the Inputs : 8 = up, 2 = down, 4 = left, 6 = right
    # @return [Integer]
    def dir4
    end
    # Indicate the direction of the Inputs : 7 = upper left, 9 = upper right, 1 = lower left, 3 = lower right or dir4
    # @return [Integer]
    def dir8
    end
    # Return the entered text by the user
    # @note If you press CTRL+Letter the result is "\u0001" + A - Letter, ex : CTRL+C = "\u0003"
    # @return [String, nil] nil means no text has been entered
    def get_text
    end
    # Indicate if a joypad id is connected
    # @param id [Integer] id of the joypad
    # @return [Boolean]
    def joy_connected?(id)
    end
    # Return the number of button of a joypad
    # @param id [Integer] id of the joypad
    # @return [Integer] number of button on the joypad
    def joy_button_count(id)
    end
    # Indicate if the Joypad has a particular Axis
    # @param id [Integer] id of the joypad
    # @param axis [Integer] one of the JoyAxis constant
    # @return [Boolean]
    def joy_has_axis?(id, axis)
    end
    # Indicate if a button is pressed on a Joypad
    # @param id [Integer] id of the joypad
    # @param button [Integer] id of the button
    # @return [Boolean]
    def joy_button_press?(id, button)
    end
    # Return the value of a specific axis on the joypad
    # @param id [Integer] id of the joypad
    # @param axis [Integer] one of the JoyAxis constant
    # @return [Integer] a value between -100 and 100.
    def joy_axis_position(id, axis)
    end
    class << self
      # ID of the main joypad (for dir4 and dir8)
      # @return [Integer] id of the joypad in SFML
      attr_accessor :main_joy
      # Axis used for the right / left movement
      # @return [Integer] one of the JoyAxis constant
      attr_accessor :x_axis
      # Axis used for the up / down movement
      # @return [Integer] one of the JoyAxis constant
      attr_accessor :y_axis
    end
    # Joypad X axis
    JoyAxisX = 0
    # Joypad Y axis
    JoyAxisY = 1
    # Joypad Z axis
    JoyAxisZ = 2
    # Joypad R axis
    JoyAxisR = 3
    # Joypad U axis
    JoyAxisU = 4
    # Joypad V axis
    JoyAxisV = 5
    # Joypad povX axis
    JoyAxisPovX = 6
    # Joypad povY axis
    JoyAxisPovY = 7
    # Module that holds the key id of the keyboard according to SFML
    module Keyboard
      # sf::Keyboard::A
      A = Integer(sf::Keyboard::A)
      # sf::Keyboard::B
      B = Integer(sf::Keyboard::B)
      # sf::Keyboard::C
      C = Integer(sf::Keyboard::C)
      # sf::Keyboard::D
      D = Integer(sf::Keyboard::D)
      # sf::Keyboard::E
      E = Integer(sf::Keyboard::E)
      # sf::Keyboard::F
      F = Integer(sf::Keyboard::F)
      # sf::Keyboard::G
      G = Integer(sf::Keyboard::G)
      # sf::Keyboard::H
      H = Integer(sf::Keyboard::H)
      # sf::Keyboard::I
      I = Integer(sf::Keyboard::I)
      # sf::Keyboard::J
      J = Integer(sf::Keyboard::J)
      # sf::Keyboard::K
      K = Integer(sf::Keyboard::K)
      # sf::Keyboard::L
      L = Integer(sf::Keyboard::L)
      # sf::Keyboard::M
      M = Integer(sf::Keyboard::M)
      # sf::Keyboard::N
      N = Integer(sf::Keyboard::N)
      # sf::Keyboard::O
      O = Integer(sf::Keyboard::O)
      # sf::Keyboard::P
      P = Integer(sf::Keyboard::P)
      # sf::Keyboard::Q
      Q = Integer(sf::Keyboard::Q)
      # sf::Keyboard::R
      R = Integer(sf::Keyboard::R)
      # sf::Keyboard::S
      S = Integer(sf::Keyboard::S)
      # sf::Keyboard::T
      T = Integer(sf::Keyboard::T)
      # sf::Keyboard::U
      U = Integer(sf::Keyboard::U)
      # sf::Keyboard::V
      V = Integer(sf::Keyboard::V)
      # sf::Keyboard::W
      W = Integer(sf::Keyboard::W)
      # sf::Keyboard::X
      X = Integer(sf::Keyboard::X)
      # sf::Keyboard::Y
      Y = Integer(sf::Keyboard::Y)
      # sf::Keyboard::Z
      Z = Integer(sf::Keyboard::Z)
      # sf::Keyboard::Num0
      Num0 = Integer(sf::Keyboard::Num0)
      # sf::Keyboard::Num1
      Num1 = Integer(sf::Keyboard::Num1)
      # sf::Keyboard::Num2
      Num2 = Integer(sf::Keyboard::Num2)
      # sf::Keyboard::Num3
      Num3 = Integer(sf::Keyboard::Num3)
      # sf::Keyboard::Num4
      Num4 = Integer(sf::Keyboard::Num4)
      # sf::Keyboard::Num5
      Num5 = Integer(sf::Keyboard::Num5)
      # sf::Keyboard::Num6
      Num6 = Integer(sf::Keyboard::Num6)
      # sf::Keyboard::Num7
      Num7 = Integer(sf::Keyboard::Num7)
      # sf::Keyboard::Num8
      Num8 = Integer(sf::Keyboard::Num8)
      # sf::Keyboard::Num9
      Num9 = Integer(sf::Keyboard::Num9)
      # sf::Keyboard::Escape
      Escape = Integer(sf::Keyboard::Escape)
      # sf::Keyboard::LControl
      LControl = Integer(sf::Keyboard::LControl)
      # sf::Keyboard::LShift
      LShift = Integer(sf::Keyboard::LShift)
      # sf::Keyboard::LAlt
      LAlt = Integer(sf::Keyboard::LAlt)
      # sf::Keyboard::LSystem
      LSystem = Integer(sf::Keyboard::LSystem)
      # sf::Keyboard::RControl
      RControl = Integer(sf::Keyboard::RControl)
      # sf::Keyboard::RShift
      RShift = Integer(sf::Keyboard::RShift)
      # sf::Keyboard::RAlt
      RAlt = Integer(sf::Keyboard::RAlt)
      # sf::Keyboard::RSystem
      RSystem = Integer(sf::Keyboard::RSystem)
      # sf::Keyboard::Menu
      Menu = Integer(sf::Keyboard::Menu)
      # sf::Keyboard::LBracket
      LBracket = Integer(sf::Keyboard::LBracket)
      # sf::Keyboard::RBracket
      RBracket = Integer(sf::Keyboard::RBracket)
      # sf::Keyboard::SemiColon
      SemiColon = Integer(sf::Keyboard::SemiColon)
      # sf::Keyboard::Comma
      Comma = Integer(sf::Keyboard::Comma)
      # sf::Keyboard::Period
      Period = Integer(sf::Keyboard::Period)
      # sf::Keyboard::Quote
      Quote = Integer(sf::Keyboard::Quote)
      # sf::Keyboard::Slash
      Slash = Integer(sf::Keyboard::Slash)
      # sf::Keyboard::BackSlash
      BackSlash = Integer(sf::Keyboard::BackSlash)
      # sf::Keyboard::Tilde
      Tilde = Integer(sf::Keyboard::Tilde)
      # sf::Keyboard::Equal
      Equal = Integer(sf::Keyboard::Equal)
      # sf::Keyboard::Dash
      Dash = Integer(sf::Keyboard::Dash)
      # sf::Keyboard::Space
      Space = Integer(sf::Keyboard::Space)
      # sf::Keyboard::Return
      Return = Integer(sf::Keyboard::Return)
      # sf::Keyboard::BackSpace
      BackSpace = Integer(sf::Keyboard::BackSpace)
      # sf::Keyboard::Tab
      Tab = Integer(sf::Keyboard::Tab)
      # sf::Keyboard::PageUp
      PageUp = Integer(sf::Keyboard::PageUp)
      # sf::Keyboard::PageDown
      PageDown = Integer(sf::Keyboard::PageDown)
      # sf::Keyboard::End
      End = Integer(sf::Keyboard::End)
      # sf::Keyboard::Home
      Home = Integer(sf::Keyboard::Home)
      # sf::Keyboard::Insert
      Insert = Integer(sf::Keyboard::Insert)
      # sf::Keyboard::Delete
      Delete = Integer(sf::Keyboard::Delete)
      # sf::Keyboard::Add
      Add = Integer(sf::Keyboard::Add)
      # sf::Keyboard::Subtract
      Subtract = Integer(sf::Keyboard::Subtract)
      # sf::Keyboard::Multiply
      Multiply = Integer(sf::Keyboard::Multiply)
      # sf::Keyboard::Divide
      Divide = Integer(sf::Keyboard::Divide)
      # sf::Keyboard::Left
      Left = Integer(sf::Keyboard::Left)
      # sf::Keyboard::Right
      Right = Integer(sf::Keyboard::Right)
      # sf::Keyboard::Up
      Up = Integer(sf::Keyboard::Up)
      # sf::Keyboard::Down
      Down = Integer(sf::Keyboard::Down)
      # sf::Keyboard::Numpad0
      Numpad0 = Integer(sf::Keyboard::Numpad0)
      # sf::Keyboard::Numpad1
      Numpad1 = Integer(sf::Keyboard::Numpad1)
      # sf::Keyboard::Numpad2
      Numpad2 = Integer(sf::Keyboard::Numpad2)
      # sf::Keyboard::Numpad3
      Numpad3 = Integer(sf::Keyboard::Numpad3)
      # sf::Keyboard::Numpad4
      Numpad4 = Integer(sf::Keyboard::Numpad4)
      # sf::Keyboard::Numpad5
      Numpad5 = Integer(sf::Keyboard::Numpad5)
      # sf::Keyboard::Numpad6
      Numpad6 = Integer(sf::Keyboard::Numpad6)
      # sf::Keyboard::Numpad7
      Numpad7 = Integer(sf::Keyboard::Numpad7)
      # sf::Keyboard::Numpad8
      Numpad8 = Integer(sf::Keyboard::Numpad8)
      # sf::Keyboard::Numpad9
      Numpad9 = Integer(sf::Keyboard::Numpad9)
      # sf::Keyboard::F1
      F1 = Integer(sf::Keyboard::F1)
      # sf::Keyboard::F2
      F2 = Integer(sf::Keyboard::F2)
      # sf::Keyboard::F3
      F3 = Integer(sf::Keyboard::F3)
      # sf::Keyboard::F4
      F4 = Integer(sf::Keyboard::F4)
      # sf::Keyboard::F5
      F5 = Integer(sf::Keyboard::F5)
      # sf::Keyboard::F6
      F6 = Integer(sf::Keyboard::F6)
      # sf::Keyboard::F7
      F7 = Integer(sf::Keyboard::F7)
      # sf::Keyboard::F8
      F8 = Integer(sf::Keyboard::F8)
      # sf::Keyboard::F9
      F9 = Integer(sf::Keyboard::F9)
      # sf::Keyboard::F10
      F10 = Integer(sf::Keyboard::F10)
      # sf::Keyboard::F11
      F11 = Integer(sf::Keyboard::F11)
      # sf::Keyboard::F12
      F12 = Integer(sf::Keyboard::F12)
      # sf::Keyboard::F13
      F13 = Integer(sf::Keyboard::F13)
      # sf::Keyboard::F14
      F14 = Integer(sf::Keyboard::F14)
      # sf::Keyboard::F15
      F15 = Integer(sf::Keyboard::F15)
      # sf::Keyboard::Pause
      Pause = Integer(sf::Keyboard::Pause)
      module_function
      # Detect if a key is pressed
      # @param key [Integer]
      def press?(key)
      end
    end
  end
  # Module that helps to retreive Mouse information during the game time
  # 
  # The buttons of the mouse are : :LEFT (:left), :MIDDLE (:middle), :RIGHT (:right), :X1, :X2
  module Mouse
    module_function
    # Tell if a button is pressed
    # @param sym [Symbol] symbol of the button
    # @return [Boolean]
    def press?(sym)
    end
    # Tell if a button has been triggered
    # @param sym [Symbol] symbol of the button
    # @return [Boolean]
    def trigger?(sym)
    end
    # Tell if a button has been released
    # @param sym [Symbol] symbol of the button
    # @return [Boolean]
    def released?(sym)
    end
    class << self
      # Retreive the x mouse position on the screen
      # @return [Integer]
      attr_reader :x
      # Retreive the y mouse position on the screen
      # @return [Integer]
      attr_reader :y
      # The vertical mouse wheel delta
      # @return [Integer]
      attr_accessor :wheel
    end
  end
  # BlendMode applicable to a ShaderedSprite
  class BlendMode
    # Add equation : Pixel = Src * SrcFactor + Dst * DstFactor
    Add = sf::BlendMode::Equation::Add
    # Substract equation : Pixel = Src * SrcFactor - Dst * DstFactor
    Subtract = sf::BlendMode::Equation::Subtract
    # Reverse substract equation : Pixel = Dst * DstFactor - Src * SrcFactor.
    ReverseSubtract = sf::BlendMode::Equation::ReverseSubtract
    # Zero factor : (0, 0, 0, 0)
    Zero = sf::BlendMode::Factor::Zero
    # One factor : (1, 1, 1, 1)
    One = sf::BlendMode::Factor::One
    # Src color factor : (src.r, src.g, src.b, src.a)
    SrcColor = sf::BlendMode::Factor::SrcColor
    # One minus src color factor : (1, 1, 1, 1) - (src.r, src.g, src.b, src.a)
    OneMinusSrcColor = sf::BlendMode::Factor::OneMinusSrcColor
    # Dest color factor : (dst.r, dst.g, dst.b, dst.a)
    DstColor = sf::BlendMode::Factor::DstColor
    # One minus dest color factor : (1, 1, 1, 1) - (dst.r, dst.g, dst.b, dst.a)
    OneMinusDstColor = sf::BlendMode::Factor::OneMinusDstColor
    # Src alpha factor : (src.a, src.a, src.a, src.a)
    SrcAlpha = sf::BlendMode::Factor::SrcAlpha
    # One minus src alpha factor : (1, 1, 1, 1) - (src.a, src.a, src.a, src.a)
    OneMinusSrcAlpha = sf::BlendMode::Factor::OneMinusSrcAlpha
    # Dest alpha factor : (dst.a, dst.a, dst.a, dst.a)
    DstAlpha = sf::BlendMode::Factor::DstAlpha
    # One minus dest alpha factor : (1, 1, 1, 1) - (dst.a, dst.a, dst.a, dst.a)
    OneMinusDstAlpha = sf::BlendMode::Factor::OneMinusDstAlpha
    # Return the source color factor
    # @return [Integer]
    attr_accessor :color_src_factor
    # Return the destination color factor
    # @return [Integer]
    attr_accessor :color_dest_factor
    # Return the source alpha factor
    # @return [Integer]
    attr_accessor :alpha_src_factor
    # Return the destination alpha factor
    # @return [Integer]
    attr_accessor :alpha_dest_factor
    # Return the color equation
    # @return [Integer]
    attr_accessor :color_equation
    # Return the alpha equation
    # @return [Integer]
    attr_accessor :alpha_equation
    # Set the RMXP blend_type : 0 = normal, 1 = addition, 2 = substraction
    # @return [Integer]
    attr_accessor :blend_type
  end
  # Shader loaded applicable to a ShaderedSprite
  class Shader < BlendMode
    # Define a Fragment shader
    Fragment = sf::Shader::Type::Fragment
    # Define a Vertex shader
    Vertex = sf::Shader::Type::Vertex
    # Define a Geometry shader
    Geometry = sf::Shader::Type::Geometry
    # @overload load(fragment_code)
    #   Load a fragment shader from memory
    #   @param framgment_code [String] shader code of the fragment shader
    # @overload load(code, type)
    #   Load a shader from memory
    #   @param code [String] the code of the shader
    #   @param type [Integer] the type of shader (Fragment, Vertex, Geometry)
    # @overload load(vertex_code, fragment_code)
    #   Load a vertex and fragment shader from memory
    #   @param vertex_code [String]
    #   @param fragment_code [String]
    # @overload load(vertex_code, geometry_code, fragment_code)
    #   Load a full shader from memory
    #   @param vertex_code [String]
    #   @param geometry_code [String]
    #   @param fragment_code [String]
    def load(*args)
    end
    alias initialize load
    # Set a Float type uniform
    # @param name [String] name of the uniform
    # @param uniform [Float, Array<Float>, LiteRGSS::Color, LiteRGSS::Tone] Array must have 2, 3 or 4 Floats
    def set_float_uniform(name, uniform)
    
    end
    # Set a Integer type uniform
    # @param name [String] name of the uniform
    # @param uniform [Integer, Array<Integer>] Array must have 2, 3 or 4 Integers
    def set_int_uniform(name, uniform)
    
    end
    # Set a Boolean type uniform
    # @param name [String] name of the uniform
    # @param uniform [Boolean, Array<Boolean>]  Array must have 2, 3 or 4 Booleans
    def set_bool_uniform(name, uniform)
    
    end
    # Set a Texture type uniform
    # @param name [String] name of the uniform
    # @param uniform [LiteRGSS::Bitmap, nil] nil means sf::Shader::CurrentTexture
    def set_texture_uniform(name, uniform)
    
    end
    # Set a Matrix type uniform (3x3 or 4x4)
    # @param name [String] name of the uniform
    # @param uniform [Array<Float>] Array must be 9 for 3x3 matrix or 16 for 4x4 matrix
    def set_matrix_uniform(name, uniform)
    
    end
    # Set a Float Array type uniform
    # @param name [String] name of the uniform
    # @param uniform [Array<Float>]
    def set_float_array_uniform(name, uniform)
    
    end
  end
  # Class that describe a Shadered Sprite
  class ShaderedSprite < Sprite
    # Set the sprite shader
    # @return [Shader, BlendMode]
    attr_accessor :shader
    # Set the sprite BlendMode
    # @return [Shader, BlendMode]
    attr_accessor :blendmode
  end
end
# Module of things made by Nuri Yuri
module Yuki
  # Get the clipboard contents
  # @return [String, nil] nil if no clipboard or incompatible clipboard
  def get_clipboard
  end
  # Set the clipboard text contents
  # @param text [String]
  def set_clipboard(text)
  
  end
  # Class that helps to read Gif
  class GifReader
    # Return the width of the Gif image
    # @return [Integer]
    attr_reader :width
    # Return the height of the Gif image
    # @return [Integer]
    attr_reader :height
    # Return the frame index of the Gif image
    # @return [Integer]
    attr_accessor :frame
    # Return the number of frame in the Gif image
    # @return [Integer]
    attr_reader :frame_count
    # Create a new GifReader
    # @param filenameordata [String]
    # @param from_memory [Boolean]
    def initialize(filenameordata, from_memory = false)
    
    end
    # Update the gif animation
    # @param bitmap [LiteRGSS::Bitmap] texture that receive the update
    # @return [self]
    def update(bitmap)
    end
    # Draw the current frame in a bitmap
    # @param bitmap [LiteRGSS::Bitmap] texture that receive the frame
    # @return [self]
    def draw(bitmap)
    end
    # Set the delta counter used to count frames
    # @param value [Numeric] the number of miliseconds per frame
    def self.delta_counter=(value)
    end
    # Describe an error that happend during gif processing
    class Error < StandardError
    end
  end
end
# Class that store a 3D array of value coded with 16bits (signed)
class Table
  # Create a new table without pre-initialization of the contents
  # @param xsize [Integer] number of row
  # @param ysize [Integer] number of cols
  # @param zsize [Integer] number of 2D table
  # @note Never call initialize from the Ruby code other than using Table.new. It'll create memory if you call initialize from Ruby, use #resize instead.
  def initialize(xsize, ysize = 1, zsize = 1)
  end
  # Access to a value of the table
  # @param x [Integer] index of the row
  # @param y [Integer] index of the col
  # @param z [Integer] index of the 2D table
  # @return [Integer, nil] nil if x, y or z are outside of the table.
  def [](x, y = 0, z = 0)
  end
  # Change a value in the table
  # @overload []=(x, value)
  #   @param x [Integer] row to affect to the new value
  #   @param value [Integer] new value
  # @overload []=(x, y, value)
  #   @param x [Integer] row index of the cell to affect to the new value
  #   @param y [Integer] col index of the cell to affect to the new value
  #   @param value [Integer] new value
  # @overload []=(x, y, z, value)
  #   @param x [Integer] row index of the cell to affect to the new value
  #   @param y [Integer] col index of the cell to affect to the new value
  #   @param z [Integer] index of the table containing the cell to affect to the new value
  #   @param value [Integer] new value
  def []=(x, y, z, value)
  end
  # @return [Integer] number of row in the table
  attr_reader :xsize
  # @return [Integer] number of cols in the table
  attr_reader :ysize
  # @return [Integer] number of 2D table in the table
  attr_reader :zsize
  # @return [Integer] Dimension of the table (1D, 2D, 3D)
  attr_reader :dim
  # Resize the table
  # @param xsize [Integer] number of row
  # @param ysize [Integer] number of cols
  # @param zsize [Integer] number of 2D table
  # @note Some value may be undeterminated if the new size is bigger than the old size
  def resize(xsize, ysize = 1, zsize = 1)
  end
  # Fill the whole table with a specific value
  # @param value [Integer] the value to affect to every cell of the table
  def fill(value)
  end
  # Copy another table to this table
  # @param table [Table] the other table
  # @param dest_offset_x [Integer] index of the row that will receive the first row of the other table
  # @param dest_offset_y [Integer] index of the col that will receive the first colum of the other table
  # @return [Boolean] if the operation was done
  # @note If any parameter is invalid (eg. dest_offset_coord < 0) the function does nothing.
  def copy(table, dest_offset_x, dest_offset_y)
  end
  # Copy another table to a specified surface of the current table using a circular copy (dest_coord = offset + source_coord % source_size)
  # @param table [Table] the other table
  # @param source_origin_x [Integer] index of the first row to copy in the current table
  # @param source_origin_y [Integer] index of the first col to copy in the current table
  # @param dest_offset_x [Integer] index of the row that will receive the first row of the other table
  # @param dest_offset_y [Integer] index of the col that will receive the first colum of the other table
  # @param width [Integer] width of the destination surface that receive the other table values
  # @param height [Integer] height of the destination surface that receive the other table values
  def copy_modulo(table, source_origin_x, source_origin_y, dest_offset_x, dest_offset_y, width, height)
  end
end
# Class that store a 3D array of value coded with 32bits (signed)
class Table32
  # Create a new table without pre-initialization of the contents
  # @param xsize [Integer] number of row
  # @param ysize [Integer] number of cols
  # @param zsize [Integer] number of 2D table
  # @note Never call initialize from the Ruby code other than using Table.new. It'll create memory if you call initialize from Ruby, use #resize instead.
  def initialize(xsize, ysize = 1, zsize = 1)
  end
  # Access to a value of the table
  # @param x [Integer] index of the row
  # @param y [Integer] index of the col
  # @param z [Integer] index of the 2D table
  # @return [Integer, nil] nil if x, y or z are outside of the table.
  def [](x, y = 0, z = 0)
  end
  # Change a value in the table
  # @overload []=(x, value)
  #   @param x [Integer] row to affect to the new value
  #   @param value [Integer] new value
  # @overload []=(x, y, value)
  #   @param x [Integer] row index of the cell to affect to the new value
  #   @param y [Integer] col index of the cell to affect to the new value
  #   @param value [Integer] new value
  # @overload []=(x, y, z, value)
  #   @param x [Integer] row index of the cell to affect to the new value
  #   @param y [Integer] col index of the cell to affect to the new value
  #   @param z [Integer] index of the table containing the cell to affect to the new value
  #   @param value [Integer] new value
  def []=(x, y, z, value)
  end
  # @return [Integer] number of row in the table
  attr_reader :xsize
  # @return [Integer] number of cols in the table
  attr_reader :ysize
  # @return [Integer] number of 2D table in the table
  attr_reader :zsize
  # @return [Integer] Dimension of the table (1D, 2D, 3D)
  attr_reader :dim
  # Resize the table
  # @param xsize [Integer] number of row
  # @param ysize [Integer] number of cols
  # @param zsize [Integer] number of 2D table
  # @note Some value may be undeterminated if the new size is bigger than the old size
  def resize(xsize, ysize = 1, zsize = 1)
  end
  # Fill the whole table with a specific value
  # @param value [Integer] the value to affect to every cell of the table
  def fill(value)
  end
=begin
  # Copy another table to this table
  # @param table [Table] the other table
  # @param dest_offset_x [Integer] index of the row that will receive the first row of the other table
  # @param dest_offset_y [Integer] index of the col that will receive the first colum of the other table
  # @return [Boolean] if the operation was done
  # @note If any parameter is invalid (eg. dest_offset_coord < 0) the function does nothing.
  def copy(table, dest_offset_x, dest_offset_y)
  end
  # Copy another table to a specified surface of the current table using a circular copy (dest_coord = offset + source_coord % source_size)
  # @param table [Table] the other table
  # @param source_origin_x [Integer] index of the first row to copy in the current table
  # @param source_origin_y [Integer] index of the first col to copy in the current table
  # @param dest_offset_x [Integer] index of the row that will receive the first row of the other table
  # @param dest_offset_y [Integer] index of the col that will receive the first colum of the other table
  # @param width [Integer] width of the destination surface that receive the other table values
  # @param height [Integer] height of the destination surface that receive the other table values
  def copy_modulo(table, source_origin_x, source_origin_y, dest_offset_x, dest_offset_y, width, height)
  end
=end
end