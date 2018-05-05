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
    def transition(nb_frame = Config::FrameRate, bitmap = nil)
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
  # Class that stores an image loaded from file or memory
  class Bitmap
    # Create a new bitmap
    # @param filename_or_memory [String]
    # @param from_memory [Boolean]
    def initialize(filename_or_memory, from_memory = false)
    end
    # Free the bitmap
    # @return [self]
    def dispose
    end
    # Indicate if the bitmap is freed or not
    # @return [Boolean]
    def disposed?
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
  end
  # Class that describes a surface of the screen where texts and sprites are shown (with some global effect)
  class Viewport
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
    # Dispose the viewport (and all its sprites/texts)
    # @return [self]
    def dispose
    end
    # Indicate if the viewport is disposed or not
    # @return [Boolean]
    def disposed?
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
  class Sprite
    # Create a new Sprite
    # @param viewport [Viewport, nil] the viewport in which the sprite is shown
    def initialize(viewport = nil)
    end
    # Dispose the sprite
    # @return [self]
    def dispose
    end
    # Indicate if the sprite is disposed or not
    # @return [Boolean]
    def disposed?
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
    # @return [LiteRGSS::Viewport, nil]
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
  class Text
    # Create a new Text
    # @param font_id [Integer] the id of the font to use to draw the text (loads the size and default colors from that)
    # @param viewport [Viewport, nil] the viewport in which the text is shown
    # @param x [Integer] the x coordinate of the text surface
    # @param y [Integer] the y coordinate of the text surface
    # @param width [Integer] the width of the text surface
    # @param height [Integer] the height of the text surface
    # @param str [String] the text shown by this object
    # @param align [0, 1, 2] the align of the text in its surface (best effort => no resize), 0 = left, 1 = center, 2 = right
    # @param outlinesize [Integer, nil] the size of the text outline
    def initialize(font_id, viewport, x, y, width, height, str, align = 0, outlinesize = nil)
    end
    # Dispose the text
    # @return [self]
    def dispose
    end
    # Indicate if the text is disposed or not
    # @return [Boolean]
    def disposed?
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
    # @return [LiteRGSS::Viewport, nil]
    attr_reader :viewport
    # If the text should be shown in italic
    # @return [Boolean]
    attr_accessor :italic
    # If the text should be shown in bold
    # @return [Boolean]
    attr_accessor :bold
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
  # Shader loaded applicable to a ShaderedSprite
  class Shader
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
    # @param uniform [Float, Array<Float>] Array must have 2, 3 or 4 Floats
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
    # @return [Shader]
    attr_accessor :shader
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
